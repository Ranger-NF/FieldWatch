const { initializeApp } = require("firebase/app");
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

const app = initializeApp(firebaseConfig);
const db = app.database();

const animalRef = db.ref("animals");

animalRef.on("child_added", (dataEvent) => {
  data = dataEvent.val();
  console.log("New user added:", data);
});
