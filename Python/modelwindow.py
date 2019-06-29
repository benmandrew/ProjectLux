import tkinter as tk
from modeldata import ModelData

class ModelWindow(tk.Toplevel):

    def __init__(self, parent, model):
        self.parent = parent
        self.model = model
        self.validModelArguments = True
        ## initialise the tkinter superclass
        ## that MainWindow inherited from
        tk.Toplevel.__init__(self)
        self.configureWindow(model.name)
        self.setBinds()
        self.initLabels()
        self.initEntries()
        self.initButtons()

    ## Set all the required window properties
    def configureWindow(self, modelName):
        self.focus_force()
        self.title(modelName)
        self.resizable(False, False)
        self.grab_set()

    ## Bind keyboard inputs to their required functions
    def setBinds(self):
        self.bind(
            "<Escape>",
            self.destroyWindow)

    ## Initialise labels, and place them in the window
    def initLabels(self):
        tk.Label(self, text="File name (model.obj):").grid(row=0, column=0, padx=10, pady=10)
        tk.Label(self, text="Position (x, y, z):").grid(row=1, column=0, padx=10, pady=10)
        tk.Label(self, text="Rotation (degrees):").grid(row=2, column=0, padx=10, pady=10)
        tk.Label(self, text="Flip (x, y, z):").grid(row=3, column=0, padx=10, pady=10)

    ## Create all entries
    def initEntries(self):
        self.initFilenameEntry()
        self.initPosEntries()
        self.initRotEntries()
        self.initFlipChecks()

    ## Create filename entries
    def initFilenameEntry(self):
        ## Initialise entry
        self.filenameEntry = tk.Entry(self)
        ## Insert model data into entry
        self.filenameEntry.insert(0, self.model.filename)
        ## Place entry in the window
        self.filenameEntry.grid(row=0, column=1, columnspan=3, padx=10, pady=10)

    ## Create position entries
    def initPosEntries(self):
        ## Initialise entries
        self.xEntry = tk.Entry(self, width=5)
        self.yEntry = tk.Entry(self, width=5)
        self.zEntry = tk.Entry(self, width=5)
        ## Insert model data into entries
        self.xEntry.insert(0, str(self.model.x))
        self.yEntry.insert(0, str(self.model.y))
        self.zEntry.insert(0, str(self.model.z))
        ## Place entries in the window
        self.xEntry.grid(row=1, column=1, padx=10, pady=10)
        self.yEntry.grid(row=1, column=2, padx=10, pady=10)
        self.zEntry.grid(row=1, column=3, padx=10, pady=10)

    ## Create rotation entries
    def initRotEntries(self):
        ## Initialise entries
        self.rotxEntry = tk.Entry(self, width=5)
        self.rotyEntry = tk.Entry(self, width=5)
        self.rotzEntry = tk.Entry(self, width=5)
        ## Insert model data into entries
        self.rotxEntry.insert(0, str(self.model.rotx))
        self.rotyEntry.insert(0, str(self.model.roty))
        self.rotzEntry.insert(0, str(self.model.rotz))
        ## Place entries in the window
        self.rotxEntry.grid(row=2, column=1, padx=10, pady=10)
        self.rotyEntry.grid(row=2, column=2, padx=10, pady=10)
        self.rotzEntry.grid(row=2, column=3, padx=10, pady=10)

    ## Create flip check buttons
    def initFlipChecks(self):
        ## Initialise integer variables used for
        ## storing the state of the checkbuttons
        vx = tk.IntVar()
        vy = tk.IntVar()
        vz = tk.IntVar()
        ## Initialise check buttons
        self.flipxCheck = tk.Checkbutton(self, variable=vx)
        self.flipyCheck = tk.Checkbutton(self, variable=vy)
        self.flipzCheck = tk.Checkbutton(self, variable=vz)
        ## Set check button variables as the integer variables
        self.flipxCheck.var = vx
        self.flipyCheck.var = vy
        self.flipzCheck.var = vz
        ## Insert model data into check buttons
        if self.model.flipx:
            self.flipxCheck.toggle()
        if self.model.flipy:
            self.flipyCheck.toggle()
        if self.model.flipz:
            self.flipzCheck.toggle()
        ## Place check buttons in the window
        self.flipxCheck.grid(row=3, column=1, padx=10, pady=10)
        self.flipyCheck.grid(row=3, column=2, padx=10, pady=10)
        self.flipzCheck.grid(row=3, column=3, padx=10, pady=10)

    ## Create buttons
    def initButtons(self):
        ## Initialise buttons, including the
        ## functions to be called when they are clicked
        self.cancelButton = tk.Button(self, text="Cancel", command=self.destroyWindow)
        self.acceptButton = tk.Button(self, text="Accept", command=self.windowExit)
        ## Place buttons in the window
        self.cancelButton.grid(row=4, column=0, padx=10, pady=10, sticky=tk.W)
        self.acceptButton.grid(row=4, column=3, padx=10, pady=10, sticky=tk.E)

    ## return to main window if inputs are valid
    def windowExit(self):
        self.saveAndValidate()
        if self.validModelArguments:
            self.parent.updateModelName(self.model)
            self.destroyWindow()

    ## Save and validate the input data
    def saveAndValidate(self):
        self.saveModelData()
        self.validateModelData()

    ## Save entry and check button contents to the model
    def saveModelData(self):
        self.model.filename = self.filenameEntry.get()
        self.model.name = (self.model.filename.split(".")[0]).capitalize()
        self.model.x = self.xEntry.get()
        self.model.y = self.yEntry.get()
        self.model.z = self.zEntry.get()
        self.model.rotx = self.rotxEntry.get()
        self.model.roty = self.rotyEntry.get()
        self.model.rotz = self.rotzEntry.get()
        self.model.flipx = self.flipxCheck.var.get()
        self.model.flipy = self.flipyCheck.var.get()
        self.model.flipz = self.flipzCheck.var.get()

    ## Check whether the input data is valid
    def validateModelData(self):
        attrbValidityDict = self.model.getValidation()
        self.highlightInvalidEntries(attrbValidityDict)
        if False in attrbValidityDict.values():
            self.validModelArguments = False
        else:
            self.validModelArguments = True

    ## For each entry, if data is valid
    ## highlight red, if invalid highlight red
    def highlightInvalidEntries(self, attrbValidityDict):
        redBG = {"background":"Red"}
        whiteBG = {"background":"White"}
        if not attrbValidityDict["filename"]:
            self.filenameEntry.configure(redBG)
        else:
            self.filenameEntry.configure(whiteBG)

        self.highlightInvalidPosEntries(attrbValidityDict)
        self.highlightInvalidRotEntries(attrbValidityDict)

    ## Highlight position entries
    def highlightInvalidPosEntries(self, attrbValidityDict):
        redBG = {"background":"Red"}
        whiteBG = {"background":"White"}
        if not attrbValidityDict["x"]:
            self.xEntry.configure(redBG)
        else:
            self.xEntry.configure(whiteBG)
        if not attrbValidityDict["y"]:
            self.yEntry.configure(redBG)
        else:
            self.yEntry.configure(whiteBG)
        if not attrbValidityDict["z"]:
            self.zEntry.configure(redBG)
        else:
            self.zEntry.configure(whiteBG)

    ## Highlight rotation entries
    def highlightInvalidRotEntries(self, attrbValidityDict):
        redBG = {"background":"Red"}
        whiteBG = {"background":"White"}
        if not attrbValidityDict["rotx"]:
            self.rotxEntry.configure(redBG)
        else:
            self.rotxEntry.configure(whiteBG)
        if not attrbValidityDict["roty"]:
            self.rotyEntry.configure(redBG)
        else:
            self.rotyEntry.configure(whiteBG)
        if not attrbValidityDict["rotz"]:
            self.rotzEntry.configure(redBG)
        else:
            self.rotzEntry.configure(whiteBG)

    ## Quit the window, returning
    ## control to the main window
    def destroyWindow(self, event=None):
        self.grab_release()
        self.destroy()















