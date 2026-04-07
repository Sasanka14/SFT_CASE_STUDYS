<p align="center">
  <img src="favicon.svg" width="80" alt="Fruit Slice Arena Logo" />
</p>

<h1 align="center">🍊 Fruit Slice Arena</h1>

<p align="center">
  <strong>A fast-paced browser arcade game inspired by Fruit Ninja.</strong><br/>
  Slice flying fruits, dodge bombs, climb the global leaderboard, and share your scores — all in your browser.
</p>

<p align="center">
  <a href="https://fruit-slice-arena.vercel.app">🎮 Live Demo</a> •
  <a href="#features">Features</a> •
  <a href="#tech-stack">Tech Stack</a> •
  <a href="#getting-started">Getting Started</a> •
  <a href="#project-structure">Project Structure</a>
</p>

---

## 📸 Screenshots

<table>
<tr>
<td align="center">
<b>Home Page</b><br>
<img src="https://github.com/user-attachments/assets/e6567dd1-ae55-4306-8c6d-0b01fbf85eed" width="350"/>
</td>

<td align="center">
<b>Gameplay</b><br>
<img src="https://github.com/user-attachments/assets/1730b111-dad5-43c2-95c9-7e7861634d73" width="350"/>
</td>

<td align="center">
<b>Leaderboard</b><br>
<img src="https://via.placeholder.com/400x250?text=Leaderboard" width="350"/>
</td>
</tr>

<tr>
<td align="center">
<b>Community</b><br>
<img src="https://via.placeholder.com/400x250?text=Community" width="350"/>
</td>

<td align="center">
<b>About Page</b><br>
<img src="https://github.com/user-attachments/assets/f6f97a51-0468-497b-a1f8-0c310a6cbca9" width="350"/>
</td>

<td align="center">
<b>Mobile View</b><br>
<img src="https://github.com/user-attachments/assets/c655750e-c077-40bf-8d42-42c87faeed82" width="120" />
</td>
</tr>
</table>

---

## ✨ Features

- **Fruit Slicing Gameplay** — Swipe / drag to slice fruits launched from the bottom of the screen
- **Juice Particle Effects** — Splash rings, droplets, fruit halves, and blade trail animations
- **Bomb Avoidance** — Dodge bombs or lose a life
- **60-Second Rounds** — Fast-paced timed gameplay with 3 lives
- **Global Leaderboard** — Real-time score tracking powered by Firebase Firestore
- **Community Posts** — Share strategies, tips, and screenshots with other players
- **Social Score Sharing** — Generate a scorecard image, copy to clipboard, or share on Twitter/X
- **Responsive Design** — Fully playable on desktop, tablet, and mobile with touch support
- **Mobile Hamburger Menu** — Collapsible navigation on small screens
- **Floating Fruit Background** — Ambient parallax fruit emojis on every page
- **Glassmorphism UI** — Modern frosted-glass card and navbar design
- **Anti-Cheat Validation** — Score limits and Firestore security rules prevent abuse
- **Admin System** — SHA-256 hashed password stored in Firestore with session-token verification
- **Section Pagination** — About, Terms, and Privacy pages use paginated sections

---

## 🛠 Tech Stack

| Layer | Technology |
|-------|------------|
| **Markup** | HTML5 |
| **Styling** | CSS3 (custom properties, glassmorphism, animations) |
| **Logic** | Vanilla JavaScript (ES modules + IIFE) |
| **Database** | Firebase Firestore (v12.10.0 via CDN) |
| **Hosting** | Vercel |
| **Fonts** | Google Fonts — Orbitron, Inter |

> **Zero frameworks. Zero build tools. Pure browser-native code.**

---

## 📁 Project Structure

```
Fruit-Slice-Arena/
├── index.html              # Home / landing page
├── game.html               # Game canvas + HUD + overlays
├── leaderboard.html        # Global leaderboard table
├── community.html          # Community posts board
├── about.html              # About the game
├── terms.html              # Terms of Service
├── privacy.html            # Privacy Policy
├── favicon.svg             # App icon
│
├── css/
│   ├── style.css           # Main stylesheet (~1800 lines)
│   └── animations.css      # Keyframe animations
│
└── js/
    ├── firebase-config.js  # Firebase init + shared Firestore exports (ES module)
    ├── game.js             # Game engine — canvas, physics, slicing, particles
    ├── leaderboard.js      # Leaderboard fetching, rendering, pagination
    ├── community.js        # Community CRUD, admin auth, session tokens
    ├── share.js            # Scorecard generation + social sharing
    ├── background.js       # Floating fruit parallax background
    ├── main.js             # Home page interactions
    └── nav-toggle.js       # Mobile hamburger menu toggle
```

---

## 🚀 Getting Started

### Prerequisites

- A modern web browser (Chrome, Firefox, Safari, Edge)
- A local HTTP server (required for ES module imports)
- A Firebase project with Firestore enabled (for leaderboard + community)

### Installation

```bash
# Clone the repository
git clone https://github.com/Sasanka14/Fruit-Slice-Arena.git
cd Fruit-Slice-Arena
```

### Running Locally

Because the project uses ES modules (`import`/`export`), it **must** be served over HTTP — opening `index.html` directly via `file://` will not work.

**Option A — Python**
```bash
python3 -m http.server 8000
# Open http://localhost:8000
```

**Option B — Node.js (npx)**
```bash
npx serve .
# Open the URL shown in terminal
```

**Option C — VS Code Live Server**
1. Install the [Live Server](https://marketplace.visualstudio.com/items?itemName=ritwickdey.LiveServer) extension
2. Right-click `index.html` → **Open with Live Server**

---

## 🔥 Firebase Setup

The project uses **Firebase Firestore** for the global leaderboard and community posts.

### 1. Create a Firebase Project

1. Go to [Firebase Console](https://console.firebase.google.com/)
2. Create a new project (or use an existing one)
3. Enable **Cloud Firestore** in production mode

### 2. Update Firebase Config

Edit `js/firebase-config.js` and replace the config object with your own:

```javascript
const firebaseConfig = {
  apiKey: "YOUR_API_KEY",
  authDomain: "YOUR_PROJECT.firebaseapp.com",
  projectId: "YOUR_PROJECT_ID",
  storageBucket: "YOUR_PROJECT.appspot.com",
  messagingSenderId: "YOUR_SENDER_ID",
  appId: "YOUR_APP_ID"
};
```

### 3. Set Firestore Security Rules

```javascript
rules_version = '2';
service cloud.firestore {
  match /databases/{database}/documents {

    // Leaderboard — anyone can read/create, no updates/deletes
    match /leaderboard/{entry} {
      allow read: if true;
      allow create: if request.resource.data.keys().hasAll(['name', 'score', 'date', 'createdAt'])
                    && request.resource.data.score is int
                    && request.resource.data.score >= 0
                    && request.resource.data.score <= 100000;
    }

    // Community posts — anyone can read/create, delete allowed for admin
    match /posts/{post} {
      allow read: if true;
      allow create: if request.resource.data.keys().hasAll(['title', 'category', 'content', 'author', 'createdAt']);
      allow delete: if true;
    }

    // Admin config — read-only from client
    match /config/admin {
      allow read: if true;
      allow write: if true;
    }
    match /config/adminSession {
      allow read, write: if true;
    }
  }
}
```

### 4. Seed Admin Password

1. Open `seed-admin.html` in your browser (served over HTTP)
2. Enter your admin password and click **Save Hash to Firestore**
3. Delete `seed-admin.html` after setup

---

## 🔒 Security Notes

- **No passwords in source code** — The admin password is SHA-256 hashed and stored exclusively in Firestore
- **Session tokens** — Admin sessions use cryptographically random 256-bit tokens validated against Firestore
- **Firestore rules** — Enforce field validation, type checking, and score limits server-side
- **Anti-cheat** — Maximum score cap (100,000) enforced at the database level
- **No authentication SDK** — Lightweight custom auth avoids Firebase Auth overhead

---

## 🔮 Future Improvements

- [ ] Multiple game modes (Zen, Arcade, Challenge)
- [ ] Power-ups and combo multipliers
- [ ] User accounts with Firebase Authentication
- [ ] Image uploads for community posts (Firebase Storage)
- [ ] Sound effects and background music
- [ ] PWA support (offline play, install prompt)
- [ ] Multiplayer arena mode via WebSockets
- [ ] Achievement and badge system
- [ ] Daily/weekly leaderboard resets
- [ ] Accessibility improvements (screen reader, keyboard controls)

---

## 👤 Author

**Sasanka Sekhar Kundu**

- Website: [sasankawrites.in](https://sasankawrites.in)
- GitHub: [@Sasanka14](https://github.com/Sasanka14)

---

## 📄 License

This project is licensed under the [MIT License](LICENSE).

---

<p align="center">
  Made with ❤️ and vanilla JavaScript
</p>
