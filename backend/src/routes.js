const express = require("express");
const routes = express.Router();
const firebase = require("./services/firebase");
const fs = require("fs");
const formidable = require("formidable");
const { Console } = require("console");

let lastImage = "";

routes.post("/pushphoto", async (req, res) => {
  const database = firebase.database();
  const { path } = req.query;
  const ref = database.ref(path);

  console.log("Iniciando procedimento de envio de imagem.")

  const form = formidable({ multiples: true });

  let formFiles = await new Promise(function (resolve, reject) {
    form.parse(req, function (err, fields, files) {
      if (err) {
        reject(err);
        return;
      }
      resolve(files);
    });
  });

  if(formFiles){
    console.log("Formulário recebido");

    let raw = fs.readFileSync(formFiles.imageFile.path);
    let base64data = raw.toString("base64");
    image = base64data;
    
    console.log("Inicianto tentativa de envio de imagem ao firebase");
    
    await ref.push({
      base64data,
    });
    
    res.status(200).send({
      res: "ok",
    });
  } else {
    console.log("Nenhum formulário recebido");
    res.status(404).send({
      res: "Erro",
    });
  }

});

// Sobrescreve o valor de uma flag
routes.post("/setflag", async (req, res) => {
  const database = firebase.database();
  const { path, value } = req.query;
  const ref = database.ref(path);

  console.log("Alterando Flag");

  await ref.set({
    value: value,
  });

  res.status(200).send({
    path,
    value,
  });
});

// Leitura do valor de uma flag
routes.get("/readflag", async (req, res) => {
  const database = firebase.database();
  const { path } = req.query;
  let value = 0;
  const ref = database.ref(path);

  await ref.once("value", (snapshot) => {
    value = snapshot.val().value;
  });

  res.status(200).send({
    value: value,
  });
});

// Retorna a última foto da camera de live
routes.get("/readphoto", async (req, res) => {
  const database = firebase.database();
  const { path } = req.query;
  const ref = database.ref(path).orderByKey().limitToLast(1);

  let lastEntry = 0;

  await ref.once("value", (snapshot) => {
    const content = snapshot.val();
    lastEntry = Object.values(content)[0].base64data;
  });

  res.status(200).send({
    value: lastEntry,
  });
});

routes.post("/streaming", async (req, res) => {
  const form = formidable({ multiples: true });

  let formFiles = await new Promise(function (resolve, reject) {
    form.parse(req, function (err, fields, files) {
      if (err) {
        reject(err);
        return;
      }
      resolve(files);
    });
  });

  console.log(formFiles.imageFile.path);
  let raw = fs.readFileSync(formFiles.imageFile.path);
  let base64data = raw.toString("base64");
  lastImage = base64data;
});

routes.get("/getimage", async (req, res) => {
  // let raw = fs.readFileSync(formFiles.imageFile.path);
  // let base64data = raw.toString("base64");

  // return res.status(200).send({
  //   image: base64data,
  // });

  res.status(200).send({
    image: lastImage,
  });
});

routes.get("/hello", async(req, res) => {
  res.status(200).send({
    text: "Hello World"
  });
});

module.exports = routes;