import { initializeApp } from "firebase/app";
import { getDatabase, ref, onValue } from "firebase/database";
import Audic from "audic";

const firebaseConfig = {};

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
    isInitialConnect = false;
  }
});

function playMusic() {
  if (audioPlayer) {
    audioPlayer.destroy();
  }

  audioPlayer = new Audic("./sounds/sample.mp3");
  audioPlayer.play();

  console.log("Started playing");

  audioPlayer.addEventListener("ended", () => {
    audioPlayer.destroy();
  });
}
