import firebase from "firebase/app";
import "firebase/auth";
import "firebase/firestore";
import "firebase/storage";
import "firebase/database";

var firebaseConfig = {
  apiKey: process.env.REACT_APP_API_KEY,
  authDomain: process.env.REACT_APP_AUTH_DOMAIN,
  projectId: process.env.REACT_APP_PROJECT_ID,
  storageBucket: process.env.REACT_APP_STORAGE_BUCKET,
  messagingSenderId: process.env.REACT_APP_MESSAGING_SENDER_ID,
  appId: process.env.REACT_APP_APP_ID,
};

class Firebase {
  constructor() {
    // Inicializa o Firebase
    firebase.initializeApp(firebaseConfig);

    // Configura pesistência de autentificação para "local"
    firebase.auth().setPersistence(firebase.auth.Auth.Persistence.LOCAL);

    // Módulos que serão utilizados
    this.auth = firebase.auth;
    this.firestore = firebase.firestore;
    this.storage = firebase.storage;
    this.database = firebase.database;

    // firebase.functions().useFunctionsEmulator('http://localhost:5001');
  }
}

export default new Firebase();