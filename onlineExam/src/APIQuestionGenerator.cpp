#include "APIQuestionGenerator.h"
#include <curl/curl.h>
#include <cstdlib>
#include <iostream>
#include <sstream>

static size_t WriteCallback(void* contents, size_t size,
                            size_t nmemb, void* userp)
{
    ((std::string*)userp)->append(
        (char*)contents, size * nmemb);
    return size * nmemb;
}

// -------- Minimal JSON helpers (no external library) --------

static std::string jsonGetString(const std::string& json,
                                  const std::string& key)
{
    std::string search = "\"" + key + "\"";
    auto pos = json.find(search);
    if (pos == std::string::npos) return "";

    pos = json.find(':', pos + search.size());
    if (pos == std::string::npos) return "";

    pos = json.find('"', pos + 1);
    if (pos == std::string::npos) return "";

    pos++;
    std::string result;
    while (pos < json.size() && json[pos] != '"') {
        if (json[pos] == '\\' && pos + 1 < json.size()) {
            pos++;
            if (json[pos] == 'n') result += '\n';
            else if (json[pos] == 't') result += '\t';
            else result += json[pos];
        } else {
            result += json[pos];
        }
        pos++;
    }
    return result;
}

// Extract text content from Gemini response
// Gemini format: {"candidates":[{"content":{"parts":[{"text":"..."}]}}]}
static std::string extractGeminiContent(const std::string& response)
{
    auto candidatesPos = response.find("\"candidates\"");
    if (candidatesPos == std::string::npos) return "";

    auto partsPos = response.find("\"parts\"", candidatesPos);
    if (partsPos == std::string::npos) return "";

    auto textPos = response.find("\"text\"", partsPos);
    if (textPos == std::string::npos) return "";

    textPos = response.find(':', textPos + 6);
    if (textPos == std::string::npos) return "";

    textPos = response.find('"', textPos + 1);
    if (textPos == std::string::npos) return "";

    textPos++;
    std::string result;
    while (textPos < response.size() && response[textPos] != '"') {
        if (response[textPos] == '\\' && textPos + 1 < response.size()) {
            textPos++;
            if (response[textPos] == 'n') result += '\n';
            else if (response[textPos] == 't') result += '\t';
            else if (response[textPos] == '"') result += '"';
            else if (response[textPos] == '\\') result += '\\';
            else result += response[textPos];
        } else {
            result += response[textPos];
        }
        textPos++;
    }
    return result;
}

// Extract content from OpenAI response (fallback)
static std::string extractOpenAIContent(const std::string& response)
{
    auto choicesPos = response.find("\"choices\"");
    if (choicesPos == std::string::npos) return "";

    auto messagePos = response.find("\"message\"", choicesPos);
    if (messagePos == std::string::npos) return "";

    auto contentPos = response.find("\"content\"", messagePos);
    if (contentPos == std::string::npos) return "";

    contentPos = response.find(':', contentPos + 9);
    if (contentPos == std::string::npos) return "";

    contentPos = response.find('"', contentPos + 1);
    if (contentPos == std::string::npos) return "";

    contentPos++;
    std::string result;
    while (contentPos < response.size() && response[contentPos] != '"') {
        if (response[contentPos] == '\\' && contentPos + 1 < response.size()) {
            contentPos++;
            if (response[contentPos] == 'n') result += '\n';
            else if (response[contentPos] == 't') result += '\t';
            else if (response[contentPos] == '"') result += '"';
            else if (response[contentPos] == '\\') result += '\\';
            else result += response[contentPos];
        } else {
            result += response[contentPos];
        }
        contentPos++;
    }
    return result;
}

// Parse questions array from the content JSON
static std::vector<Question> parseQuestions(const std::string& content)
{
    std::vector<Question> result;

    std::string json = content;

    // Strip markdown code fences if present
    auto fenceStart = json.find("```");
    if (fenceStart != std::string::npos) {
        auto lineEnd = json.find('\n', fenceStart);
        if (lineEnd != std::string::npos)
            json = json.substr(lineEnd + 1);
        auto fenceEnd = json.rfind("```");
        if (fenceEnd != std::string::npos)
            json = json.substr(0, fenceEnd);
    }

    // Find each question object {...}
    size_t pos = 0;
    while (pos < json.size()) {
        auto objStart = json.find('{', pos);
        if (objStart == std::string::npos) break;

        int depth = 0;
        size_t objEnd = objStart;
        for (size_t i = objStart; i < json.size(); i++) {
            if (json[i] == '{') depth++;
            else if (json[i] == '}') {
                depth--;
                if (depth == 0) { objEnd = i; break; }
            }
        }

        if (depth != 0) break;

        std::string obj = json.substr(objStart, objEnd - objStart + 1);
        pos = objEnd + 1;

        std::string qText = jsonGetString(obj, "question");
        if (qText.empty()) continue;

        std::vector<std::string> options;
        auto optPos = obj.find("\"options\"");
        if (optPos == std::string::npos) continue;

        auto arrStart = obj.find('[', optPos);
        auto arrEnd = obj.find(']', arrStart);
        if (arrStart == std::string::npos || arrEnd == std::string::npos)
            continue;

        std::string arrStr = obj.substr(arrStart + 1,
                                         arrEnd - arrStart - 1);

        size_t p = 0;
        while (p < arrStr.size()) {
            auto qs = arrStr.find('"', p);
            if (qs == std::string::npos) break;
            qs++;
            std::string opt;
            while (qs < arrStr.size() && arrStr[qs] != '"') {
                if (arrStr[qs] == '\\' && qs + 1 < arrStr.size()) {
                    qs++;
                    opt += arrStr[qs];
                } else {
                    opt += arrStr[qs];
                }
                qs++;
            }
            options.push_back(opt);
            p = qs + 1;
        }

        if (options.size() < 4) continue;
        options.resize(4);

        int correctIdx = 0;
        auto caPos = obj.find("\"correct_answer\"");
        if (caPos != std::string::npos) {
            auto colonPos = obj.find(':', caPos + 16);
            if (colonPos != std::string::npos) {
                size_t numStart = colonPos + 1;
                while (numStart < obj.size() &&
                       (obj[numStart] == ' ' || obj[numStart] == '"'))
                    numStart++;
                correctIdx = std::atoi(obj.c_str() + numStart);
            }
        }

        if (correctIdx < 0 || correctIdx > 3) correctIdx = 0;

        Question q;
        q.setQuestion(qText, options, correctIdx, "medium", "api");
        result.push_back(q);
    }

    return result;
}

// -------- Gemini API call --------

static std::vector<Question> callGemini(
    const std::string& topic,
    const std::string& difficulty,
    int count)
{
    std::vector<Question> result;

    const char* apiKey = std::getenv("GEMINI_API_KEY");
    if (!apiKey) return result;

    CURL* curl = curl_easy_init();
    if (!curl) return result;

    std::string response;

    std::string prompt =
        "Generate exactly " + std::to_string(count) +
        " multiple choice questions on the topic: " + topic +
        " at " + difficulty + " difficulty level. "
        "Return ONLY a JSON array (no markdown fences, no explanation) where each "
        "object has: "
        "\\\"question\\\": the question text, "
        "\\\"options\\\": array of exactly 4 answer strings, "
        "\\\"correct_answer\\\": 0-based index of the correct option. "
        "Example: "
        "[{\\\"question\\\":\\\"What is 2+2?\\\","
        "\\\"options\\\":[\\\"3\\\",\\\"4\\\",\\\"5\\\",\\\"6\\\"],"
        "\\\"correct_answer\\\":1}]";

    // Gemini API endpoint
    std::string url =
        "https://generativelanguage.googleapis.com/v1beta/models/"
        "gemini-2.0-flash:generateContent?key=" + std::string(apiKey);

    std::string jsonData =
        "{"
        "\"contents\":[{"
        "\"parts\":[{\"text\":\"" + prompt + "\"}]"
        "}],"
        "\"generationConfig\":{"
        "\"temperature\":0.7,"
        "\"responseMimeType\":\"application/json\""
        "}"
        "}";

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers,
        "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);

    std::cout << "🔄 Fetching questions from Gemini AI...\n";
    CURLcode res = curl_easy_perform(curl);

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);

    if (res != CURLE_OK) {
        std::cerr << "❌ Gemini request failed: "
                  << curl_easy_strerror(res) << "\n";
        return result;
    }

    // Check for API error
    if (response.find("\"error\"") != std::string::npos) {
        std::string errMsg = jsonGetString(response, "message");
        std::cerr << "❌ Gemini API error: " << errMsg << "\n";
        return result;
    }

    std::string content = extractGeminiContent(response);
    if (content.empty()) {
        std::cerr << "❌ Could not parse Gemini response.\n";
        return result;
    }

    result = parseQuestions(content);
    return result;
}

// -------- OpenAI API call (fallback) --------

static std::vector<Question> callOpenAI(
    const std::string& topic,
    const std::string& difficulty,
    int count)
{
    std::vector<Question> result;

    const char* apiKey = std::getenv("OPENAI_API_KEY");
    if (!apiKey) return result;

    const char* modelEnv = std::getenv("OPENAI_MODEL");
    std::string model = modelEnv ? modelEnv : "gpt-4o-mini";

    CURL* curl = curl_easy_init();
    if (!curl) return result;

    std::string response;

    std::string prompt =
        "Generate exactly " + std::to_string(count) +
        " multiple choice questions on the topic: " + topic +
        " at " + difficulty + " difficulty level. "
        "Return ONLY a JSON array (no markdown, no explanation) where each "
        "object has: "
        "\\\"question\\\": the question text, "
        "\\\"options\\\": array of exactly 4 answer strings, "
        "\\\"correct_answer\\\": 0-based index of the correct option. "
        "Example: "
        "[{\\\"question\\\":\\\"What is 2+2?\\\","
        "\\\"options\\\":[\\\"3\\\",\\\"4\\\",\\\"5\\\",\\\"6\\\"],"
        "\\\"correct_answer\\\":1}]";

    std::string jsonData =
        "{"
        "\"model\":\"" + model + "\","
        "\"temperature\":0.7,"
        "\"messages\":["
        "{\"role\":\"system\",\"content\":\"You are a quiz generator. "
        "Return only valid JSON arrays, no markdown fences or extra text.\"},"
        "{\"role\":\"user\",\"content\":\"" + prompt + "\"}"
        "]"
        "}";

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers,
        ("Authorization: Bearer " + std::string(apiKey)).c_str());
    headers = curl_slist_append(headers,
        "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL,
        "https://api.openai.com/v1/chat/completions");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);

    std::cout << "🔄 Fetching questions from OpenAI...\n";
    CURLcode res = curl_easy_perform(curl);

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);

    if (res != CURLE_OK) {
        std::cerr << "❌ OpenAI request failed: "
                  << curl_easy_strerror(res) << "\n";
        return result;
    }

    if (response.find("\"error\"") != std::string::npos) {
        std::string errMsg = jsonGetString(response, "message");
        std::cerr << "❌ OpenAI error: " << errMsg << "\n";
        return result;
    }

    std::string content = extractOpenAIContent(response);
    if (content.empty()) {
        std::cerr << "❌ Could not parse OpenAI response.\n";
        return result;
    }

    result = parseQuestions(content);
    return result;
}

// -------- Public entry point: tries Gemini first, then OpenAI --------

std::vector<Question> APIQuestionGenerator::generateQuestions(
    const std::string& topic,
    const std::string& difficulty,
    int count)
{
    // Try Gemini first (free tier)
    std::vector<Question> result = callGemini(topic, difficulty, count);

    // Fallback to OpenAI if Gemini didn't work
    if (result.empty()) {
        const char* openaiKey = std::getenv("OPENAI_API_KEY");
        if (openaiKey) {
            std::cout << "⚡ Trying OpenAI as fallback...\n";
            result = callOpenAI(topic, difficulty, count);
        }
    }

    if (result.empty()) {
        std::cerr << "⚠️ No questions generated from any AI provider.\n";
    } else {
        std::cout << "✅ " << result.size()
                  << " questions generated from AI.\n";
    }

    return result;
}

// -------- Gemini-only public method --------

std::vector<Question> APIQuestionGenerator::generateFromGemini(
    const std::string& topic,
    const std::string& difficulty,
    int count)
{
    auto result = callGemini(topic, difficulty, count);
    if (result.empty())
        std::cerr << "⚠️ Gemini failed to generate questions.\n";
    else
        std::cout << "✅ " << result.size()
                  << " questions generated from Gemini.\n";
    return result;
}

// -------- OpenAI-only public method --------

std::vector<Question> APIQuestionGenerator::generateFromOpenAI(
    const std::string& topic,
    const std::string& difficulty,
    int count)
{
    auto result = callOpenAI(topic, difficulty, count);
    if (result.empty())
        std::cerr << "⚠️ OpenAI failed to generate questions.\n";
    else
        std::cout << "✅ " << result.size()
                  << " questions generated from OpenAI.\n";
    return result;
}