import { initializeApp } from "firebase/app";
import { getDatabase, ref, onValue } from "firebase/database";
import Audic from "audic";

const firebaseConfig = {
  apiKey: "AIzaSyAkTKmBKvvStfZf6HbhcfnyUwbPDthzSz8",
  authDomain: "the-scarecrow.firebaseapp.com",
  databaseURL:
    "https://the-scarecrow-default-rtdb.asia-southeast1.firebasedatabase.app",
  projectId: "the-scarecrow",
  storageBucket: "the-scarecrow.firebasestorage.app",
  messagingSenderId: "1024452937217",
  appId: "1:1024452937217:web:42f416d77877f924ce6200",
  measurementId: "G-L9DCTEJJRC",
};

let isInitialConnect = true;
let audioPlayer;

const app = initializeApp(firebaseConfig);
const db = getDatabase(app);

const animalRef = ref(db, "intruders");

onValue(animalRef, (snapshot) => {
  if (!isInitialConnect) {
    let data = snapshot.val();
    playMusic();
  } else {
    isInitialConnect = false
  }
});


function playMusic() {
  if (audioPlayer) {
    audioPlayer.destroy();
  }

  audioPlayer = new Audic("./sounds/sample.mp3");
  audioPlayer.play();

  console.log("Started playing");
  
  audioPlayer.addEventListener('ended', () => {
    audioPlayer.destroy();
  });
}
