import { app, BrowserWindow } from 'electron'
import {createWindow} from './utils/window'
import * as path from "path";

let win: BrowserWindow

app.on('ready', () => {
  console.log("App is ready");
  win = createWindow(600, 800)

  const indexHTML = path.join(__dirname + "/index.html");
  win
    .loadFile(indexHTML)
    .then(() => {
      // IMPLEMENT FANCY STUFF HERE
    })
    .catch((e) => console.error(e));
})
