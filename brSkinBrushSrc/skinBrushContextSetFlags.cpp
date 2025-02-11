#include "skinBrushTool.h"

// ---------------------------------------------------------------------
// setting values from the command flags
// ---------------------------------------------------------------------
void SkinBrushContext::setColorR(float value) {
    colorVal.r = value;
    MToolsInfo::setDirtyFlag(*this);
}

void SkinBrushContext::setColorG(float value) {
    colorVal.g = value;
    MToolsInfo::setDirtyFlag(*this);
}

void SkinBrushContext::setColorB(float value) {
    colorVal.b = value;
    MToolsInfo::setDirtyFlag(*this);
}

void SkinBrushContext::setCurve(int value) {
    curveVal = value;
    MToolsInfo::setDirtyFlag(*this);
}

void SkinBrushContext::setDrawBrush(bool value) {
    drawBrushVal = value;
    MToolsInfo::setDirtyFlag(*this);
}

void SkinBrushContext::setDrawRange(bool value) {
    drawRangeVal = value;
    MToolsInfo::setDirtyFlag(*this);
}

void SkinBrushContext::setPythonImportPath(MString value) {
    moduleImportString = value;
    MToolsInfo::setDirtyFlag(*this);
}

void SkinBrushContext::setEnterToolCommand(MString value) {
    enterToolCommandVal = value;
    MToolsInfo::setDirtyFlag(*this);
}

void SkinBrushContext::setExitToolCommand(MString value) {
    exitToolCommandVal = value;
    MToolsInfo::setDirtyFlag(*this);
}

void SkinBrushContext::setFlood() {
    this->verticesPainted.clear();
    this->skinValuesToSet.clear();
    double value = strengthVal;

    // 0 Add - 1 Remove - 2 AddPercent - 3 Absolute - 4 Smooth - 5 Sharpen - 6 LockVertices - 7
    // UnLockVertices
    ModifierCommands theCommandIndex = this->commandIndex;
    if (this->modifierNoneShiftControl == ModifierKeys::Shift)
        theCommandIndex = ModifierCommands::Smooth;  // smooth always
    if (this->modifierNoneShiftControl == ModifierKeys::ControlShift)
        theCommandIndex = ModifierCommands::Sharpen;  // sharpen always

    if (
        theCommandIndex == ModifierCommands::Smooth
        || this->modifierNoneShiftControl == ModifierKeys::Shift
        || this->modifierNoneShiftControl == ModifierKeys::ControlShift
    )
        value = smoothStrengthVal;

    for (int i = 0; i < this->numVertices; ++i) {
        this->verticesPainted.insert(i);
        this->skinValuesToSet.insert(std::make_pair(i, value));
    }
    doTheAction();
    if (verbose)
        MGlobal::displayInfo(MString("SET FLOOD IS CALLED command ") + static_cast<int>(theCommandIndex) +
                             MString(" value ") + value);
    MToolsInfo::setDirtyFlag(*this);
}

void SkinBrushContext::setVerbose(bool value) { verbose = value; }

void SkinBrushContext::setFractionOversampling(bool value) {
    fractionOversamplingVal = value;
    MToolsInfo::setDirtyFlag(*this);
}

void SkinBrushContext::setIgnoreLock(bool value) {
    ignoreLockVal = value;
    MToolsInfo::setDirtyFlag(*this);
}

void SkinBrushContext::setLineWidth(int value) {
    lineWidthVal = value;
    MToolsInfo::setDirtyFlag(*this);
}

void SkinBrushContext::setMessage(int value) {
    messageVal = value;
    MToolsInfo::setDirtyFlag(*this);

    setInViewMessage(true);
}

void SkinBrushContext::setOversampling(int value) {
    oversamplingVal = value;
    MToolsInfo::setDirtyFlag(*this);
}

void SkinBrushContext::setRange(double value) {
    rangeVal = value;
    MToolsInfo::setDirtyFlag(*this);
}

void SkinBrushContext::setSize(double value) {
    sizeVal = value;
    MToolsInfo::setDirtyFlag(*this);
}

void SkinBrushContext::setStrength(double value) {
    // 0 Add - 1 Remove - 2 AddPercent - 3 Absolute - 4 Smooth - 5 Sharpen - 6 LockVertices - 7
    // unlockVertices
    if (commandIndex == ModifierCommands::Smooth)
        smoothStrengthVal = value;
    else  // others
        strengthVal = value;

    MToolsInfo::setDirtyFlag(*this);
}
void SkinBrushContext::setSmoothStrength(double value) {
    smoothStrengthVal = value;
    MToolsInfo::setDirtyFlag(*this);
}

void SkinBrushContext::setPruneWeights(double value) {
    this->pruneWeight = value;
    MToolsInfo::setDirtyFlag(*this);
}

void SkinBrushContext::setInteractiveValue(double value, int ind) {
    if (ind == 0)
        this->interactiveValue = value;
    else if (ind == 1)
        this->interactiveValue1 = value;
    else if (ind == 2)
        this->interactiveValue2 = value;
    MToolsInfo::setDirtyFlag(*this);
}

void SkinBrushContext::setUndersampling(int value) {
    undersamplingVal = value;
    MToolsInfo::setDirtyFlag(*this);
}

void SkinBrushContext::setVolume(bool value) {
    volumeVal = value;
    MToolsInfo::setDirtyFlag(*this);
}

void SkinBrushContext::setMirrorTolerance(double value) {
    mirrorMinDist = value;
    MToolsInfo::setDirtyFlag(*this);
}

void SkinBrushContext::setPaintMirror(int value) {
    paintMirror = value;
    if (value != 0) {
        getTheOrigMeshForMirror();
    }
    MToolsInfo::setDirtyFlag(*this);
}

void SkinBrushContext::setUseColorSetsWhilePainting(bool value) {
    useColorSetsWhilePainting = value;
    MToolsInfo::setDirtyFlag(*this);
}

void SkinBrushContext::setDrawTriangles(bool value) {
    drawTriangles = value;
    MToolsInfo::setDirtyFlag(*this);
}

void SkinBrushContext::setDrawEdges(bool value) {
    drawEdges = value;
    MToolsInfo::setDirtyFlag(*this);
}

void SkinBrushContext::setDrawPoints(bool value) {
    drawPoints = value;
    MToolsInfo::setDirtyFlag(*this);
}

void SkinBrushContext::setDrawTransparency(bool value) {
    drawTransparency = value;
    MToolsInfo::setDirtyFlag(*this);
}

void SkinBrushContext::setMinColor(double value) {
    minSoloColor = value;
    refreshDeformerColor(this->influenceIndex);
    MToolsInfo::setDirtyFlag(*this);
}

void SkinBrushContext::setMaxColor(double value) {
    maxSoloColor = value;
    refreshDeformerColor(this->influenceIndex);
    MToolsInfo::setDirtyFlag(*this);
}

void SkinBrushContext::setCommandIndex(ModifierCommands value) {
    // MGlobal::displayInfo(MString("setCommandIndex CALLED ") + value);
    commandIndex = value;
    MToolsInfo::setDirtyFlag(*this);
}

void SkinBrushContext::setSmoothRepeat(int value) {
    // MGlobal::displayInfo(MString("setCommandIndex CALLED ") + value);
    smoothRepeat = value;
    MToolsInfo::setDirtyFlag(*this);
}

void SkinBrushContext::setSoloColor(int value) {
    // MGlobal::displayInfo(MString("setSoloColor CALLED ") + value);
    soloColorVal = value;
    MString currentColorSet = meshFn.currentColorSetName();  // set multiColor as current Color

    if (soloColorVal == 1) {  // solo
        meshFn.setCurrentColorSetName(this->soloColorSet);
        editSoloColorSet(true);
    } else {
        meshFn.setCurrentColorSetName(this->fullColorSet);
    }
    maya2019RefreshColors();
    MToolsInfo::setDirtyFlag(*this);
    //}
}

void SkinBrushContext::maya2019RefreshColors(bool toggle) {
    meshFn.updateSurface();
    view = M3dView::active3dView();
    // first swap
    if (toggle) toggleColorState = !toggleColorState;

    if (!toggle || toggleColorState) {
        if (soloColorVal == 1) {
            meshFn.setCurrentColorSetName(this->soloColorSet2);
        } else {
            meshFn.setCurrentColorSetName(this->fullColorSet2);
        }
        view.refresh(false, true);
    }
    if (!toggle || !toggleColorState) {
        if (soloColorVal == 1) {
            meshFn.setCurrentColorSetName(this->soloColorSet);
        } else {
            meshFn.setCurrentColorSetName(this->fullColorSet);
        }
        view.refresh(false, true);
    }
}

void SkinBrushContext::setSoloColorType(int value) {
    if (verbose) MGlobal::displayInfo(MString("setSoloColorType CALLED ") + value);

    if (soloColorTypeVal != value) {
        soloColorTypeVal = value;
        // here we do the redraw
        meshFn.updateSurface();
        editSoloColorSet(false);
        maya2019RefreshColors();

        MToolsInfo::setDirtyFlag(*this);
    }
}

void SkinBrushContext::setCoverage(bool value) {
    coverageVal = value;
    MToolsInfo::setDirtyFlag(*this);
}

void SkinBrushContext::setPickMaxInfluence(bool value) {
    pickMaxInfluenceVal = value;
    MToolsInfo::setDirtyFlag(*this);
}

void SkinBrushContext::setPickInfluence(bool value) {
    pickInfluenceVal = value;
    MToolsInfo::setDirtyFlag(*this);
}

void SkinBrushContext::setPostSetting(bool value) {
    MGlobal::displayInfo(MString("setPostSetting CALLED ") + value);

    postSetting = value;
    MToolsInfo::setDirtyFlag(*this);
}

void SkinBrushContext::setInfluenceIndex(int value, bool selectInUI) {
    if (verbose)
        MGlobal::displayInfo(MString("setInfluenceIndex CALLED value [") + value +
                             MString("] selectInUI [") + selectInUI + MString("]\n"));
    if (value != this->influenceIndex) {
        MString msg = MString("influence index is ") + value + MString(" inflNames.length is ") +
                      this->inflNames.length();
        if (value < this->inflNames.length()) {
            this->influenceIndex = value;

            MString influenceName = this->inflNames[value];
            msg += MString(" name is ") + influenceName;

            if (selectInUI) {
                MString pickInfluenceCommand = moduleImportString + MString("pickedInfluence\n");
                pickInfluenceCommand +=
                    MString("pickedInfluence ('") + influenceName + MString("')");
                MGlobal::executePythonCommand(pickInfluenceCommand);
            }
        }
        if (verbose) MGlobal::displayInfo(msg);
        // here we do the redraw

        if (soloColorVal == 1) {  // solo IF NOT IT CRASHES on a first pick before paint
            MString currentColorSet = meshFn.currentColorSetName();  // get current soloColor
            if (currentColorSet != this->soloColorSet)
                meshFn.setCurrentColorSetName(this->soloColorSet);
            editSoloColorSet(false);
        }

        meshFn.updateSurface();  // for proper redraw hopefully
        maya2019RefreshColors();
    }
}

void SkinBrushContext::setInfluenceByName(MString value) {
    if (verbose) MGlobal::displayInfo("setInfluenceByName CALLED \"" + value + "\"\n");
    if (this->pickMaxInfluenceVal) return;

    int indexInfluence = this->inflNames.indexOf(value);
    if (verbose)
        MGlobal::displayInfo(MString("setInfluenceByName - ") + value + MString(" ") +
                             indexInfluence);
    if (indexInfluence == -1) {
        MGlobal::displayWarning("influence not found, ERROR");
    } else {
        setInfluenceIndex(indexInfluence, false);
    }
}
void SkinBrushContext::setSkinClusterByName(MString& value) {
    if (verbose) MGlobal::displayInfo("setSkinClusterByName CALLED \"" + value + "\"\n");
    getSkinFromName = true;
    passedSkinName = value;
}
void SkinBrushContext::setMeshByName(MString& value) {
    if (verbose) MGlobal::displayInfo("setMeshByName CALLED \"" + value + "\"\n");
    getMeshFromName = true;
    passedMeshName = value;
}

// ---------------------------------------------------------------------
// getting values from the command flags
// ---------------------------------------------------------------------
float SkinBrushContext::getColorR() { return colorVal.r; }
float SkinBrushContext::getColorG() { return colorVal.g; }
float SkinBrushContext::getColorB() { return colorVal.b; }

int SkinBrushContext::getCurve() { return curveVal; }
bool SkinBrushContext::getDrawBrush() { return drawBrushVal; }
bool SkinBrushContext::getDrawRange() { return drawRangeVal; }
MString SkinBrushContext::getPythonImportPath() { return moduleImportString; }
MString SkinBrushContext::getEnterToolCommand() { return enterToolCommandVal; }
MString SkinBrushContext::getExitToolCommand() { return exitToolCommandVal; }
bool SkinBrushContext::getFractionOversampling() { return fractionOversamplingVal; }

bool SkinBrushContext::getIgnoreLock() { return ignoreLockVal; }

int SkinBrushContext::getLineWidth() { return lineWidthVal; }
int SkinBrushContext::getMessage() { return messageVal; }
int SkinBrushContext::getOversampling() { return oversamplingVal; }
double SkinBrushContext::getRange() { return rangeVal; }
double SkinBrushContext::getSize() { return sizeVal; }
double SkinBrushContext::getStrength() { return strengthVal; }
double SkinBrushContext::getSmoothStrength() { return smoothStrengthVal; }
double SkinBrushContext::getPruneWeights() { return this->pruneWeight; }

double SkinBrushContext::getInteractiveValue(int ind) {
    if (ind == 0) return this->interactiveValue;
    else if (ind == 1) return this->interactiveValue1;
    //if (ind == 2)
    return this->interactiveValue2;
}

int SkinBrushContext::getUndersampling() { return undersamplingVal; }
bool SkinBrushContext::getVolume() { return volumeVal; }
ModifierCommands SkinBrushContext::getCommandIndex() { return commandIndex; }
int SkinBrushContext::getSmoothRepeat() { return smoothRepeat; }
int SkinBrushContext::getSoloColor() { return soloColorVal; }

double SkinBrushContext::getMirrorTolerance() { return mirrorMinDist; }
int SkinBrushContext::getPaintMirror() { return paintMirror; }
bool SkinBrushContext::getUseColorSetsWhilePainting() { return useColorSetsWhilePainting; }
bool SkinBrushContext::getDrawTriangles() { return drawTriangles; }
bool SkinBrushContext::getDrawEdges() { return drawEdges; }
bool SkinBrushContext::getDrawPoints() { return drawPoints; }
bool SkinBrushContext::getDrawTransparency() { return drawTransparency; }
int SkinBrushContext::getSoloColorType() { return soloColorTypeVal; }
bool SkinBrushContext::getCoverage() { return coverageVal; }
int SkinBrushContext::getInfluenceIndex() { return influenceIndex; }

double SkinBrushContext::getMinColor() { return minSoloColor; }
double SkinBrushContext::getMaxColor() { return maxSoloColor; }

MString SkinBrushContext::getInfluenceName() {
    // MGlobal::displayInfo("getInfluenceName CALLED");
    MString influenceName("FAILED");
    if (this->influenceIndex < this->inflNames.length())
        influenceName = this->inflNames[this->influenceIndex];

    return influenceName;
}

MString SkinBrushContext::getSkinClusterName() {
    if (!skinObj.isNull()) {
        MFnDependencyNode skinDep(this->skinObj);
        return skinDep.name();
    } else {
        return MString("");
    }
}

MString SkinBrushContext::getMeshName() { return this->meshDag.fullPathName(); }
bool SkinBrushContext::getPostSetting() { return postSetting; }
