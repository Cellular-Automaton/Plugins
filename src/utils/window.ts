import { BrowserWindow } from 'electron'

const createWindow = (width: number, height: number) => {
  const win = new BrowserWindow({
    width: width,
    height: height
  })

  win.loadFile('src/index.html')
  return win
}

export { createWindow }
