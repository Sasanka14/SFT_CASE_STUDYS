const regexInput = document.getElementById('regex-input');
const testString = document.getElementById('test-string');
const highlightedText = document.getElementById('highlighted-text');
const errorMessage = document.getElementById('error-message');
const flagsDisplay = document.getElementById('flags-display');
const charCount = document.getElementById('char-count');
const matchCount = document.getElementById('match-count');
const groupCount = document.getElementById('group-count');
const stepCount = document.getElementById('step-count');
const matchesContainer = document.getElementById('matches-container');
const explanationContainer = document.getElementById('explanation-container');
const explanationContent = document.getElementById('explanation-content');
const toggleExplanation = document.getElementById('toggle-explanation');
const toggleReference = document.getElementById('toggle-reference');
const referenceContent = document.getElementById('reference-content');
const testBtn = document.getElementById('test-btn');
const clearBtn = document.getElementById('clear-btn');

// Function tabs
const functionTabs = document.querySelectorAll('.function-tab');
const replaceSection = document.getElementById('replace-section');
const replaceInput = document.getElementById('replace-input');
const resultPreviewSection = document.getElementById('result-preview-section');
const resultPreview = document.getElementById('result-preview');
const copyResultBtn = document.getElementById('copy-result');

// Export buttons
const exportJsonBtn = document.getElementById('export-json');
const exportCsvBtn = document.getElementById('export-csv');
const exportTextBtn = document.getElementById('export-text');

// Reference search
const referenceSearch = document.getElementById('reference-search');

// Flag checkboxes
const flagG = document.getElementById('flag-g');
const flagI = document.getElementById('flag-i');
const flagM = document.getElementById('flag-m');
const flagS = document.getElementById('flag-s');
const flagU = document.getElementById('flag-u');

// Current function mode
let currentFunction = 'match';

// Store current matches for export
let currentMatches = [];

// Regex Token Explanations
const tokenExplanations = {
    // Anchors
    '^': { text: 'Start of string (or line with multiline flag)', type: 'anchor' },
    '$': { text: 'End of string (or line with multiline flag)', type: 'anchor' },
    '\\b': { text: 'Word boundary', type: 'anchor' },
    '\\B': { text: 'Non-word boundary', type: 'anchor' },
    
    // Character Classes
    '.': { text: 'Any character except newline', type: 'class' },
    '\\d': { text: 'Digit [0-9]', type: 'class' },
    '\\D': { text: 'Non-digit [^0-9]', type: 'class' },
    '\\w': { text: 'Word character [a-zA-Z0-9_]', type: 'class' },
    '\\W': { text: 'Non-word character', type: 'class' },
    '\\s': { text: 'Whitespace character', type: 'class' },
    '\\S': { text: 'Non-whitespace character', type: 'class' },
    
    // Quantifiers
    '*': { text: 'Match 0 or more times (greedy)', type: 'quantifier' },
    '+': { text: 'Match 1 or more times (greedy)', type: 'quantifier' },
    '?': { text: 'Match 0 or 1 time (optional)', type: 'quantifier' },
    '*?': { text: 'Match 0 or more times (lazy)', type: 'quantifier' },
    '+?': { text: 'Match 1 or more times (lazy)', type: 'quantifier' },
    '??': { text: 'Match 0 or 1 time (lazy)', type: 'quantifier' },
    
    // Escape sequences
    '\\n': { text: 'Newline character', type: 'escape' },
    '\\r': { text: 'Carriage return', type: 'escape' },
    '\\t': { text: 'Tab character', type: 'escape' },
    '\\0': { text: 'Null character', type: 'escape' },
    '\\\\': { text: 'Literal backslash', type: 'escape' },
    '\\.': { text: 'Literal dot', type: 'escape' },
    '\\*': { text: 'Literal asterisk', type: 'escape' },
    '\\+': { text: 'Literal plus', type: 'escape' },
    '\\?': { text: 'Literal question mark', type: 'escape' },
    '\\^': { text: 'Literal caret', type: 'escape' },
    '\\$': { text: 'Literal dollar sign', type: 'escape' },
    '\\[': { text: 'Literal opening bracket', type: 'escape' },
    '\\]': { text: 'Literal closing bracket', type: 'escape' },
    '\\(': { text: 'Literal opening parenthesis', type: 'escape' },
    '\\)': { text: 'Literal closing parenthesis', type: 'escape' },
    '\\{': { text: 'Literal opening brace', type: 'escape' },
    '\\}': { text: 'Literal closing brace', type: 'escape' },
    '\\|': { text: 'Literal pipe', type: 'escape' },
    
    // Groups
    '|': { text: 'Alternation (OR)', type: 'group' },
};

// Utility Functions

/**
 * @param {string} text - Text to escape
 * @returns {string} - Escaped text
 */
function escapeHtml(text) {
    const div = document.createElement('div');
    div.textContent = text;
    return div.innerHTML;
}

/**
 * @returns {string} - Flags string
 */

function getFlags() {
    let flags = '';
    if (flagG.checked) flags += 'g';
    if (flagI.checked) flags += 'i';
    if (flagM.checked) flags += 'm';
    if (flagS.checked) flags += 's';
    if (flagU.checked) flags += 'u';
    return flags;
}

/**
 * Create a RegExp object safely
 * @param {string} pattern - Regex pattern
 * @param {string} flags - Regex flags
 * @returns {RegExp|null} - RegExp object or null if invalid
 */
function createRegex(pattern, flags) {
    try {
        return new RegExp(pattern, flags);
    } catch (e) {
        return null;
    }
}

/**
 * Show error message
 * @param {string} message - Error message to display
 */
function showError(message) {
    errorMessage.textContent = message;
    errorMessage.classList.add('visible');
}

// Hide error message
function hideError() {
    errorMessage.textContent = '';
    errorMessage.classList.remove('visible');
}

// Pattern Explanation Parser

/**
 * Parse regex pattern and generate explanations
 * @param {string} pattern - Regex pattern to explain
 * @returns {Array} - Array of explanation objects
 */
function parsePattern(pattern) {
    const explanations = [];
    let i = 0;
    
    while (i < pattern.length) {
        const char = pattern[i];
        const nextChar = pattern[i + 1];
        const twoChars = char + (nextChar || '');
        
        // Check for escape sequences first
        if (char === '\\' && nextChar) {
            if (tokenExplanations[twoChars]) {
                explanations.push({
                    token: twoChars,
                    ...tokenExplanations[twoChars]
                });
                i += 2;
                continue;
            } else if (/\d/.test(nextChar)) {
                // Backreference
                explanations.push({
                    token: twoChars,
                    text: `Backreference to group ${nextChar}`,
                    type: 'group'
                });
                i += 2;
                continue;
            } else {
                // Generic escape
                explanations.push({
                    token: twoChars,
                    text: `Literal character "${nextChar}"`,
                    type: 'escape'
                });
                i += 2;
                continue;
            }
        }
        
        // Character class [...]
        if (char === '[') {
            let classEnd = i + 1;
            let negated = false;
            
            if (pattern[classEnd] === '^') {
                negated = true;
                classEnd++;
            }
            
            while (classEnd < pattern.length && pattern[classEnd] !== ']') {
                if (pattern[classEnd] === '\\') classEnd++;
                classEnd++;
            }
            
            const classContent = pattern.slice(i, classEnd + 1);
            explanations.push({
                token: classContent,
                text: negated 
                    ? `Match any character NOT in: ${classContent.slice(2, -1)}`
                    : `Match any character in: ${classContent.slice(1, -1)}`,
                type: 'class'
            });
            i = classEnd + 1;
            continue;
        }
        
        // Groups (...) and (?:...) and (?=...) etc
        if (char === '(') {
            if (pattern.slice(i, i + 3) === '(?:') {
                explanations.push({
                    token: '(?:',
                    text: 'Non-capturing group start',
                    type: 'group'
                });
                i += 3;
                continue;
            } else if (pattern.slice(i, i + 3) === '(?=') {
                explanations.push({
                    token: '(?=',
                    text: 'Positive lookahead start',
                    type: 'group'
                });
                i += 3;
                continue;
            } else if (pattern.slice(i, i + 3) === '(?!') {
                explanations.push({
                    token: '(?!',
                    text: 'Negative lookahead start',
                    type: 'group'
                });
                i += 3;
                continue;
            } else if (pattern.slice(i, i + 4) === '(?<=') {
                explanations.push({
                    token: '(?<=',
                    text: 'Positive lookbehind start',
                    type: 'group'
                });
                i += 4;
                continue;
            } else if (pattern.slice(i, i + 4) === '(?<!') {
                explanations.push({
                    token: '(?<!',
                    text: 'Negative lookbehind start',
                    type: 'group'
                });
                i += 4;
                continue;
            } else if (pattern.slice(i, i + 3).match(/\(\?<[^>]/)) {
                // Named capture group (?<name>...)
                const nameMatch = pattern.slice(i).match(/\(\?<([^>]+)>/);
                if (nameMatch) {
                    explanations.push({
                        token: nameMatch[0],
                        text: `Named capturing group "${nameMatch[1]}"`,
                        type: 'group'
                    });
                    i += nameMatch[0].length;
                    continue;
                }
            } else {
                explanations.push({
                    token: '(',
                    text: 'Capturing group start',
                    type: 'group'
                });
                i++;
                continue;
            }
        }
        
        if (char === ')') {
            explanations.push({
                token: ')',
                text: 'Group end',
                type: 'group'
            });
            i++;
            continue;
        }
        
        // Quantifiers with {n} or {n,} or {n,m}
        if (char === '{') {
            const quantMatch = pattern.slice(i).match(/^\{(\d+)(,)?(\d*)?\}/);
            if (quantMatch) {
                const [full, min, comma, max] = quantMatch;
                let text;
                if (!comma) {
                    text = `Match exactly ${min} times`;
                } else if (!max) {
                    text = `Match ${min} or more times`;
                } else {
                    text = `Match between ${min} and ${max} times`;
                }
                explanations.push({
                    token: full,
                    text,
                    type: 'quantifier'
                });
                i += full.length;
                continue;
            }
        }
        
        // Lazy quantifiers
        if (twoChars === '*?' || twoChars === '+?' || twoChars === '??') {
            explanations.push({
                token: twoChars,
                ...tokenExplanations[twoChars]
            });
            i += 2;
            continue;
        }
        
        // Simple tokens
        if (tokenExplanations[char]) {
            explanations.push({
                token: char,
                ...tokenExplanations[char]
            });
            i++;
            continue;
        }
        
        // Literal characters
        explanations.push({
            token: char,
            text: `Literal character "${char}"`,
            type: 'literal'
        });
        i++;
    }
    
    return explanations;
}

// Match Highlighting

/**
 * Highlight matches in the test string
 * @param {string} text - Test string
 * @param {RegExp} regex - Compiled regex
 * @returns {string} - HTML with highlighted matches
 */
function highlightMatches(text, regex) {
    if (!regex || !text) {
        return escapeHtml(text);
    }
    
    const matches = [];
    let match;
    
    // Clone regex to ensure lastIndex is reset
    const regexClone = new RegExp(regex.source, regex.flags);
    
    // Handle non-global regex
    if (!regex.global) {
        match = regexClone.exec(text);
        if (match) {
            matches.push({
                index: match.index,
                length: match[0].length,
                value: match[0]
            });
        }
    } else {
        // Handle global regex
        while ((match = regexClone.exec(text)) !== null) {
            matches.push({
                index: match.index,
                length: match[0].length,
                value: match[0]
            });
            
            // Prevent infinite loop for zero-length matches
            if (match[0].length === 0) {
                regexClone.lastIndex++;
            }
        }
    }
    
    // Build highlighted HTML
    if (matches.length === 0) {
        return escapeHtml(text);
    }
    
    let result = '';
    let lastIndex = 0;
    
    for (const m of matches) {
        // Add text before match
        result += escapeHtml(text.slice(lastIndex, m.index));
        // Add highlighted match
        result += `<span class="match">${escapeHtml(m.value)}</span>`;
        lastIndex = m.index + m.length;
    }
    
    // Add remaining text
    result += escapeHtml(text.slice(lastIndex));
    
    return result;
}

// Display Functions

/**
 * Display match results
 * @param {string} text - Test string
 * @param {RegExp} regex - Compiled regex
 */
function displayMatches(text, regex) {
    if (!regex || !text) {
        matchesContainer.innerHTML = `
            <div class="empty-state">
                <div class="empty-icon">🔍</div>
                <p>Enter a regex pattern and test string to see matches</p>
            </div>
        `;
        matchCount.textContent = '0';
        groupCount.textContent = '0';
        return;
    }
    
    const matches = [];
    let match;
    let totalGroups = 0;
    
    // Clone regex
    const regexClone = new RegExp(regex.source, regex.flags);
    
    if (!regex.global) {
        match = regexClone.exec(text);
        if (match) {
            matches.push(match);
            totalGroups = match.length - 1;
        }
    } else {
        while ((match = regexClone.exec(text)) !== null) {
            matches.push(match);
            totalGroups = Math.max(totalGroups, match.length - 1);
            
            if (match[0].length === 0) {
                regexClone.lastIndex++;
            }
        }
    }
    
    matchCount.textContent = matches.length.toString();
    groupCount.textContent = totalGroups.toString();
    
    if (matches.length === 0) {
        matchesContainer.innerHTML = `
            <div class="empty-state">
                <div class="empty-icon">😕</div>
                <p>No matches found</p>
            </div>
        `;
        return;
    }
    
    let html = '';
    
    matches.forEach((m, idx) => {
        const endIndex = m.index + m[0].length;
        
        html += `
            <div class="match-item">
                <div class="match-header">
                    <span class="match-number">Match ${idx + 1}</span>
                    <span class="match-position">Index: ${m.index} - ${endIndex}</span>
                </div>
                <div class="match-value">${escapeHtml(m[0]) || '(empty string)'}</div>
        `;
        
        // Display groups
        if (m.length > 1) {
            html += '<div class="match-groups">';
            for (let i = 1; i < m.length; i++) {
                html += `
                    <div class="group-item">
                        <span class="group-label">Group ${i}:</span>
                        <span class="group-value">${m[i] !== undefined ? escapeHtml(m[i]) : '(no match)'}</span>
                    </div>
                `;
            }
            html += '</div>';
        }
        
        // Display named groups if present
        if (m.groups && Object.keys(m.groups).length > 0) {
            html += '<div class="match-groups">';
            for (const [name, value] of Object.entries(m.groups)) {
                html += `
                    <div class="group-item">
                        <span class="group-label">${escapeHtml(name)}:</span>
                        <span class="group-value">${value !== undefined ? escapeHtml(value) : '(no match)'}</span>
                    </div>
                `;
            }
            html += '</div>';
        }
        
        html += '</div>';
    });
    
    // Store matches for export
    currentMatches = matches.map((m, idx) => ({
        match: m[0],
        index: m.index,
        endIndex: m.index + m[0].length,
        groups: m.slice(1),
        namedGroups: m.groups || {}
    }));
    
    matchesContainer.innerHTML = html;
}

/**
 * Display pattern explanation
 * @param {string} pattern - Regex pattern
 */
function displayExplanation(pattern) {
    if (!pattern) {
        explanationContainer.innerHTML = `
            <div class="empty-state">
                <div class="empty-icon">📖</div>
                <p>Pattern breakdown will appear here</p>
            </div>
        `;
        return;
    }
    
    const explanations = parsePattern(pattern);
    
    if (explanations.length === 0) {
        explanationContainer.innerHTML = `
            <div class="empty-state">
                <div class="empty-icon">📖</div>
                <p>Pattern breakdown will appear here</p>
            </div>
        `;
        return;
    }
    
    let html = '<div class="explanation-list">';
    
    for (const exp of explanations) {
        const typeClass = `token-${exp.type}`;
        html += `
            <div class="explanation-item">
                <span class="explanation-token ${typeClass}">${escapeHtml(exp.token)}</span>
                <span class="explanation-text">${escapeHtml(exp.text)}</span>
            </div>
        `;
    }
    
    html += '</div>';
    explanationContainer.innerHTML = html;
}

// Main Update Function

/**
 * Main function to update all displays
 */
function updateAll() {
    const pattern = regexInput.value;
    const text = testString.value;
    const flags = getFlags();
    
    // Update flags display
    flagsDisplay.textContent = flags || '(none)';
    
    // Update character count
    charCount.textContent = `${text.length} characters`;
    
    // Try to create regex
    if (!pattern) {
        hideError();
        highlightedText.innerHTML = escapeHtml(text);
        displayMatches(text, null);
        displayExplanation('');
        stepCount.textContent = '0';
        return;
    }
    
    const regex = createRegex(pattern, flags);
    
    if (!regex) {
        // Show error
        try {
            new RegExp(pattern, flags);
        } catch (e) {
            showError(e.message);
        }
        highlightedText.innerHTML = escapeHtml(text);
        displayMatches(text, null);
        displayExplanation(pattern);
        stepCount.textContent = '0';
        return;
    }
    
    hideError();
    
    // Update all displays based on current function
    switch (currentFunction) {
        case 'match':
            highlightedText.innerHTML = highlightMatches(text, regex);
            displayMatches(text, regex);
            resultPreviewSection.style.display = 'none';
            break;
        case 'replace':
            highlightedText.innerHTML = highlightMatches(text, regex);
            displayMatches(text, regex);
            displayReplaceResult(text, regex);
            break;
        case 'split':
            highlightedText.innerHTML = highlightMatches(text, regex);
            displayMatches(text, regex);
            displaySplitResult(text, regex);
            break;
        case 'test':
            highlightedText.innerHTML = highlightMatches(text, regex);
            displayTestResult(text, regex);
            resultPreviewSection.style.display = 'none';
            break;
    }
    
    displayExplanation(pattern);
    
    // Estimate steps (simplified)
    stepCount.textContent = Math.max(1, pattern.length * (currentMatches.length || 1)).toString();
}

// Function-specific displays

/**
 * Display replace result
 */
function displayReplaceResult(text, regex) {
    const replacement = replaceInput.value;
    try {
        const result = text.replace(regex, replacement);
        resultPreviewSection.style.display = 'block';
        resultPreview.textContent = result;
    } catch (e) {
        resultPreview.textContent = 'Error: ' + e.message;
    }
}

/**
 * Display split result
 */
function displaySplitResult(text, regex) {
    try {
        const parts = text.split(regex);
        resultPreviewSection.style.display = 'block';
        resultPreview.innerHTML = parts.map((p, i) => 
            `<span style="background: ${i % 2 === 0 ? 'var(--group-1)' : 'var(--group-2)'}; padding: 2px 4px; border-radius: 4px; margin: 2px;">${escapeHtml(p) || '(empty)'}</span>`
        ).join('');
    } catch (e) {
        resultPreview.textContent = 'Error: ' + e.message;
    }
}

/**
 * Display test result
 */
function displayTestResult(text, regex) {
    const result = regex.test(text);
    matchesContainer.innerHTML = `
        <div class="test-result ${result ? 'success' : 'failure'}">
            <div class="test-icon">${result ? '✅' : '❌'}</div>
            <div class="test-text">${result ? 'Pattern matches!' : 'No match found'}</div>
        </div>
    `;
    matchCount.textContent = result ? '1' : '0';
    groupCount.textContent = '0';
}
// Export Functions

/**
 * Export matches as JSON
 */
function exportAsJson() {
    const data = {
        pattern: regexInput.value,
        flags: getFlags(),
        testString: testString.value,
        matches: currentMatches.map(m => ({
            match: m[0],
            index: m.index,
            groups: Array.from(m).slice(1),
            namedGroups: m.groups || {}
        }))
    };
    downloadFile(JSON.stringify(data, null, 2), 'regex-matches.json', 'application/json');
}

/**
 * Export matches as CSV
 */
function exportAsCsv() {
    if (currentMatches.length === 0) {
        alert('No matches to export');
        return;
    }
    
    const headers = ['Match #', 'Value', 'Start Index', 'End Index'];
    const maxGroups = Math.max(...currentMatches.map(m => m.length - 1));
    for (let i = 1; i <= maxGroups; i++) {
        headers.push(`Group ${i}`);
    }
    
    let csv = headers.join(',') + '\n';
    
    currentMatches.forEach((m, idx) => {
        const row = [
            idx + 1,
            `"${m[0].replace(/"/g, '""')}"`,
            m.index,
            m.index + m[0].length
        ];
        for (let i = 1; i < m.length; i++) {
            row.push(`"${(m[i] || '').replace(/"/g, '""')}"`);
        }
        csv += row.join(',') + '\n';
    });
    
    downloadFile(csv, 'regex-matches.csv', 'text/csv');
}

/**
 * Export matches as plain text
 */
function exportAsText() {
    let text = `Regex Pattern: /${regexInput.value}/${getFlags()}\n`;
    text += `Test String: ${testString.value.length} characters\n`;
    text += `Total Matches: ${currentMatches.length}\n\n`;
    text += '='.repeat(50) + '\n\n';
    
    currentMatches.forEach((m, idx) => {
        text += `Match ${idx + 1}:\n`;
        text += `  Value: "${m[0]}"\n`;
        text += `  Position: ${m.index} - ${m.index + m[0].length}\n`;
        if (m.length > 1) {
            for (let i = 1; i < m.length; i++) {
                text += `  Group ${i}: "${m[i] || ''}"\n`;
            }
        }
        text += '\n';
    });
    
    downloadFile(text, 'regex-matches.txt', 'text/plain');
}

/**
 * Download file helper
 */
function downloadFile(content, filename, mimeType) {
    const blob = new Blob([content], { type: mimeType });
    const url = URL.createObjectURL(blob);
    const a = document.createElement('a');
    a.href = url;
    a.download = filename;
    document.body.appendChild(a);
    a.click();
    document.body.removeChild(a);
    URL.revokeObjectURL(url);
}

// Sync Scroll for Highlighted Text

/**
 * Sync scroll position between textarea and highlight div
 */
function syncScroll() {
    highlightedText.scrollTop = testString.scrollTop;
    highlightedText.scrollLeft = testString.scrollLeft;
}

// Reference Toggle & Search

/**
 * Toggle quick reference visibility
 */
function toggleReferenceSection() {
    referenceContent.classList.toggle('collapsed');
    const icon = toggleReference.querySelector('.toggle-icon');
    icon.classList.toggle('collapsed');
}

/**
 * Toggle explanation visibility
 */
function toggleExplanationSection() {
    explanationContent.classList.toggle('collapsed');
    const icon = toggleExplanation.querySelector('.toggle-icon');
    icon.classList.toggle('collapsed');
}

/**
 * Filter reference content
 */
function filterReference(query) {
    const categories = document.querySelectorAll('.reference-category');
    const lowerQuery = query.toLowerCase();
    
    categories.forEach(cat => {
        const items = cat.querySelectorAll('li');
        let hasVisible = false;
        
        items.forEach(item => {
            const text = item.textContent.toLowerCase();
            if (text.includes(lowerQuery)) {
                item.style.display = '';
                hasVisible = true;
            } else {
                item.style.display = 'none';
            }
        });
        
        cat.style.display = hasVisible ? '' : 'none';
    });
}

/**
 * Clear all inputs and reset the application
 */
function clearAll() {
    regexInput.value = '';
    testString.value = '';
    replaceInput.value = '';
    flagG.checked = true;
    flagI.checked = false;
    flagM.checked = false;
    flagS.checked = false;
    flagU.checked = false;
    currentMatches = [];
    updateAll();
    regexInput.focus();
}

/**
 * Copy result to clipboard
 */
function copyResult() {
    const text = resultPreview.textContent;
    navigator.clipboard.writeText(text).then(() => {
        copyResultBtn.textContent = 'Copied!';
        setTimeout(() => {
            copyResultBtn.textContent = 'Copy';
        }, 2000);
    });
}

// Event Listeners

// Input events
regexInput.addEventListener('input', updateAll);
testString.addEventListener('input', updateAll);
replaceInput.addEventListener('input', updateAll);

// Flag change events
[flagG, flagI, flagM, flagS, flagU].forEach(flag => {
    flag.addEventListener('change', updateAll);
});

// Scroll sync
testString.addEventListener('scroll', syncScroll);

// Reference toggle
toggleReference.addEventListener('click', toggleReferenceSection);

// Explanation toggle
toggleExplanation.addEventListener('click', toggleExplanationSection);

// Reference search
referenceSearch.addEventListener('input', (e) => filterReference(e.target.value));

// Test button - triggers update
testBtn.addEventListener('click', updateAll);

// Clear button - resets everything
clearBtn.addEventListener('click', clearAll);

// Export buttons
exportJsonBtn.addEventListener('click', exportAsJson);
exportCsvBtn.addEventListener('click', exportAsCsv);
exportTextBtn.addEventListener('click', exportAsText);

// Copy result
copyResultBtn.addEventListener('click', copyResult);

// Function tabs
functionTabs.forEach(tab => {
    tab.addEventListener('click', () => {
        functionTabs.forEach(t => t.classList.remove('active'));
        tab.classList.add('active');
        currentFunction = tab.dataset.function;
        
        // Show/hide replace input
        replaceSection.style.display = currentFunction === 'replace' ? 'block' : 'none';
        
        updateAll();
    });
});

// Initialization

/**
 * Initialize the application with example data
 */
function init() {
    // Set example pattern and test string
    regexInput.value = '\\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Z|a-z]{2,}\\b';
    testString.value = `Hello! Here are some sample email addresses to test:

valid@email.com
another.valid+email@sub.domain.org
user123@company.co.uk
invalid-email@
@missing-local.com
test.user@domain.com

Feel free to edit this text or the regex pattern above!`;
    
    // Initial update
    updateAll();
}

// Run initialization when DOM is ready
document.addEventListener('DOMContentLoaded', init);

// Also run if DOM is already loaded
if (document.readyState !== 'loading') {
    init();
}
