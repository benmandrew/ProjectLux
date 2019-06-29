import tkinter as tk
import sys

from modeldata import ModelData
from modelwindow import ModelWindow

import RayTracer

class MainWindow(tk.Tk):

    def __init__(self):
        self.modelList = list()
        self.validArguments = True
        ## initialise the tkinter superclass
        ## that MainWindow inherited from
        tk.Tk.__init__(self)
        self.configureWindow()
        self.setBinds()
        self.initLabels()
        self.initEntries()
        self.initListboxes()
        self.initButtons()

    ## Set all the required window properties
    def configureWindow(self):
        self.title("Ray Tracer")
        self.resizable(False, False)

    ## Bind keyboard inputs to their required functions
    def setBinds(self):
        self.bind(
            "<Escape>",
            self.quitGUI)

    ## Initialise labels, and place them in the window
    def initLabels(self):
        tk.Label(self, text="Width:").grid(row=0, column=1, columnspan=3)
        tk.Label(self, text="Height:").grid(row=2, column=1, columnspan=3)
        tk.Label(self, text="Camera FOV (degrees):").grid(row=4, column=1, columnspan=3)
        tk.Label(self, text="Camera position (x, y, z):").grid(row=6, column=1, columnspan=3)

    ## Create all entries
    def initEntries(self):
        ## Initialise entries
        self.widthEntry  = tk.Entry(self, width=12)
        self.heightEntry = tk.Entry(self, width=12)
        self.camFOVEntry = tk.Entry(self, width=12)
        self.camxEntry = tk.Entry(self, width=6)
        self.camyEntry = tk.Entry(self, width=6)
        self.camzEntry = tk.Entry(self, width=6)
        ## Insert default data into the entries
        self.widthEntry.insert(0, "1600")
        self.heightEntry.insert(0, "900")
        self.camFOVEntry.insert(0, "25.0")
        self.camxEntry.insert(0, "0.0")
        self.camyEntry.insert(0, "0.0")
        self.camzEntry.insert(0, "-10.0")
        ## Place entries in the window
        self.widthEntry.grid(row=1, column=1, columnspan=3)
        self.heightEntry.grid(row=3, column=1, columnspan=3)
        self.camFOVEntry.grid(row=5, column=1, columnspan=3)
        self.camxEntry.grid(row=7, column=1)
        self.camyEntry.grid(row=7, column=2)
        self.camzEntry.grid(row=7, column=3)

    ## Create listboxes
    def initListboxes(self):
        ## Initialise listbox
        self.listbox = tk.Listbox(self, width=15)
        ## Get default model data and insert into the listbox
        defaultModel = self.getDefaultModel()
        self.listbox.insert(tk.END, defaultModel.name)
        self.modelList.append(defaultModel)
        ## Bind keyboard inputs to their required functions
        self.listbox.bind(
            "<Double-Button>",
            self.spawnModelWindow)
        self.listbox.bind(
            "<Return>",
            self.spawnModelWindow)
        self.listbox.bind(
            "<Delete>",
            self.deleteModel)
        ## Place listbox in the window
        self.listbox.grid(row=0, column=0, rowspan=6, padx=10)

    ## Create buttons
    def initButtons(self):
        ## Initialise buttons, including the
        ## functions to be called when they are clicked
        self.startButton = tk.Button(self, text="Start", command=self.start)
        self.exitButton = tk.Button(self, text="Exit", command=self.quitGUI)
        self.addButton = tk.Button(self, text="Add", command=self.addModel)
        self.deleteButton = tk.Button(self, text="Delete", command=self.deleteModel)
        ## Place buttons in the window
        self.startButton.grid(row=8, column=0, padx=10, pady=10, sticky=tk.E)
        self.exitButton.grid(row=8, column=0, padx=10, pady=10, sticky=tk.W)
        self.addButton.grid(row=6, column=0, padx=10, pady=10, sticky=tk.E)
        self.deleteButton.grid(row=6, column=0, padx=10, pady=10, sticky=tk.W)

    ## Start the renderer if inputs are valid
    def start(self):
        self.validate()
        if self.validArguments:
            self.initRender()

    ## Validate data in the entries
    def validate(self):
        attrbValidityDict = self.getValidation()
        self.highlightInvalidEntries(attrbValidityDict)
        if False in attrbValidityDict.values():
            self.validArguments = False
        else:
            self.validArguments = True

    ## Create dictionary that stores whether attributes are valid or not
    def createAttrbValidityDict(self):
        attrbValidityDict = {
            "camx":True, "camy":True, "camz":True,
            "camfov":True, "width":True, "height":True
        }
        return attrbValidityDict

    ## Check for validity of each entry,
    ## storing results in a validity dictionary
    def getValidation(self):
        attrbValidityDict = self.createAttrbValidityDict()
        if not self.widthEntry.get().isdigit():
            attrbValidityDict["width"] = False
        if not self.heightEntry.get().isdigit():
            attrbValidityDict["height"] = False
        if not self.validFloat(self.camxEntry.get()):
            attrbValidityDict["camx"] = False
        if not self.validFloat(self.camyEntry.get()):
            attrbValidityDict["camy"] = False
        if not self.validFloat(self.camzEntry.get()):
            attrbValidityDict["camz"] = False
        if not self.validFOV(self.camFOVEntry.get()):
            attrbValidityDict["camfov"] = False
        return attrbValidityDict

    ## Validate whether a string could be
    ## converted to a valid floating point number
    def validFloat(self, numString):
        try:
            float(numString)
        except ValueError:
            return False
        return True

    ## Validate whether a string could be
    ## converted to a valid camera FOV
    def validFOV(self, fovString):
        if not self.validFloat(fovString):
            return False
        fov = float(fovString)
        if not (0.0 <= fov  < 180.0):
            return False
        return True

    ## For each entry, if data is valid
    ## highlight red, if invalid highlight red
    def highlightInvalidEntries(self, attrbValidityDict):
        ## Cache the wierd dictionary arguments
        ## that Tkinter uses to configure entries
        redBG = {"background":"Red"}
        whiteBG = {"background":"White"}
        if not attrbValidityDict["width"]:
            self.widthEntry.configure(redBG)
        else:
            self.widthEntry.configure(whiteBG)
        if not attrbValidityDict["height"]:
            self.heightEntry.configure(redBG)
        else:
            self.heightEntry.configure(whiteBG)
        if not attrbValidityDict["camx"]:
            self.camxEntry.configure(redBG)
        else:
            self.camxEntry.configure(whiteBG)
        if not attrbValidityDict["camy"]:
            self.camyEntry.configure(redBG)
        else:
            self.camyEntry.configure(whiteBG)
        if not attrbValidityDict["camz"]:
            self.camzEntry.configure(redBG)
        else:
            self.camzEntry.configure(whiteBG)
        if not attrbValidityDict["camfov"]:
            self.camFOVEntry.configure(redBG)
        else:
            self.camFOVEntry.configure(whiteBG)

    ## Create a ModelWindow for the selected model
    def spawnModelWindow(self, event):
        ## Get the index of the selected model in the listbox
        index = int(event.widget.curselection()[0])
        modelWin = ModelWindow(self, self.modelList[index])

    ## Fully quit the program
    def quitGUI(self, event=None):
        ## Quit Tkinter
        self.destroy()
        ## Quit Python
        sys.exit()

    ## Delete the selected models
    def deleteModel(self):
        ## Get indices of selected models
        indices = self.listbox.curselection()
        ## Delete the models iterating backwards
        ## through the list, otherwise when the list
        ## shifts all elements left after the deleted
        ## element forwards, it would screw with the
        ## indices of the models still to be deleted
        for index in reversed(indices):
            model = self.modelList[index]
            ## Remove the model both from the
            ## listbox, and the internal list
            self.modelList.remove(model)
            self.listbox.delete(index)

    ## Get default configuration for new models
    def getDefaultModel(self):
        return ModelData(
            "sphere.obj",
            0.0, 0.0, 0.0,
            0.0, 0.0, 0.0,
            False, False, False)

    ## Add the default model, and spawn
    ## a ModelWindow to configure it
    def addModel(self):
        index = len(self.modelList)
        defaultModel = self.getDefaultModel()
        self.listbox.insert(tk.END, defaultModel.name)
        self.modelList.append(defaultModel)
        modelWin = ModelWindow(self, self.modelList[index])

    ## Update the listed name of a given model in the listbox
    def updateModelName(self, model):
        index = self.modelList.index(model)
        self.listbox.delete(index)
        self.listbox.insert(index, model.name)

    ## Pack window and model attributes
    ## to be passed to the C++ program
    def packArguments(self):
        ## Pack non-model attributes
        args = [
            self.getWidth(),
            self.getHeight(),
            self.getCamx(),
            self.getCamy(),
            self.getCamz(),
            self.getCamFOV()
        ]
        ## For each model, add the model's attributes to the list
        for model in self.modelList:
            args += model.packArguments()
        ## Convert the list to a tuple, and return.
        ## For whatever reason, C++ extensions
        ## only accept tuple packed arguments
        return tuple(args)

    ## Start the renderer with the required arguments
    def initRender(self):
        RayTracer.render(
            self.packArguments()
        )

    def getWidth(self):
        return int(self.widthEntry.get())

    def getHeight(self):
        return int(self.heightEntry.get())

    def getCamx(self):
        return float(self.camxEntry.get())

    def getCamy(self):
        return float(self.camyEntry.get())

    def getCamz(self):
        return float(self.camzEntry.get())

    def getCamFOV(self):
        return float(self.camFOVEntry.get())
        
























