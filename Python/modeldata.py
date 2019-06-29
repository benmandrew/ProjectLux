
class ModelData:

    def __init__(self, filename,
                 x, y, z,
                 rotx, roty, rotz,
                 flipx, flipy, flipz):
        self.filename = filename
        self.x = x
        self.y = y
        self.z = z
        self.rotx = rotx
        self.roty = roty
        self.rotz = rotz
        self.flipx = flipx
        self.flipy = flipy
        self.flipz = flipz
        self.name = (filename.split(".")[0]).capitalize()

    ## Create dictionary that stores whether attributes are valid or not
    def createAttrbValidityDict(self):
        attrbValidityDict = {
            "x":True, "y":True, "z":True,
            "rotx":True, "roty":True, "rotz":True,
            "filename":True
        }
        return attrbValidityDict

    ## Check for validity of each attribute,
    ## storing results in a validity dictionary
    def getValidation(self):
        attrbValidityDict = self.createAttrbValidityDict()
        if not self.validFilename(self.filename):
            attrbValidityDict["filename"] = False
        if not self.validFloat(self.x):
            attrbValidityDict["x"] = False
        if not self.validFloat(self.y):
            attrbValidityDict["y"] = False
        if not self.validFloat(self.z):
            attrbValidityDict["z"] = False
        if not self.validFloat(self.rotx):
            attrbValidityDict["rotx"] = False
        if not self.validFloat(self.roty):
            attrbValidityDict["roty"] = False
        if not self.validFloat(self.rotz):
            attrbValidityDict["rotz"] = False
        return attrbValidityDict

    ## Check whether the given filename is syntactically
    ## valid (not whether it exists on the computer)
    def validFilename(self, filename):
        if not filename.lower().endswith(".obj"):
            return False
        elif not len(filename.split(".")) == 2:
            return False
        return True

    ## Validate whether a string could be
    ## converted to a valid floating point number
    def validFloat(self, numString):
        try:
            float(numString)
        except ValueError:
            return False
        return True

    ## Pack attributes to be passed to the C++ program
    def packArguments(self):
        return [
            self.filename,
            float(self.x),
            float(self.y),
            float(self.z),
            float(self.rotx),
            float(self.roty),
            float(self.rotz),
            bool(self.flipx),
            bool(self.flipy),
            bool(self.flipz)
        ]









