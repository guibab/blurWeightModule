// ---------------------------------------------------------------------
//
//  skinBrushTool.h
//  skinBrushTool
//
//  Created by ingo on 11/18/18.
//  Copyright (c) 2018 Ingo Clemens. All rights reserved.
//
// ---------------------------------------------------------------------
#ifndef __skinBrushTool__skinBrushTool__
#define __skinBrushTool__skinBrushTool__

#include "enums.h"
#include "functions.h"
#include "setOverloads.h"

#include <math.h>
#include <maya/M3dView.h>
#include <maya/MArgDatabase.h>
#include <maya/MArgList.h>
#include <maya/MCursor.h>
#include <maya/MDagPath.h>
#include <maya/MDagPathArray.h>
#include <maya/MEulerRotation.h>
#include <maya/MEvent.h>
#include <maya/MFloatMatrix.h>
#include <maya/MFloatPointArray.h>
#include <maya/MFnCamera.h>
#include <maya/MFnDoubleArrayData.h>
#include <maya/MFnDoubleIndexedComponent.h>
#include <maya/MFnMatrixData.h>
#include <maya/MFnMesh.h>
#include <maya/MFnNurbsSurface.h>
#include <maya/MFnSingleIndexedComponent.h>
#include <maya/MFnSkinCluster.h>
#include <maya/MFrameContext.h>
#include <maya/MGlobal.h>
#include <maya/MItDependencyGraph.h>
#include <maya/MItMeshEdge.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItMeshVertex.h>
#include <maya/MItSelectionList.h>
#include <maya/MMatrix.h>
#include <maya/MMeshIntersector.h>
#include <maya/MPointArray.h>
#include <maya/MPxContext.h>
#include <maya/MPxContextCommand.h>
#include <maya/MPxToolCommand.h>
#include <maya/MSelectionList.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>
#include <maya/MSyntax.h>
#include <maya/MThreadUtils.h>
#include <maya/MToolsInfo.h>
#include <maya/MUIDrawManager.h>
#include <maya/MUintArray.h>

#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>  //std::iota
#include <set>
#include <unordered_map>
#include <vector>

// Macro for the press/drag/release methods in case there is nothing
// selected or the tool gets applied outside any geometry. If the actual
// MStatus would get returned an error can get listed in terminal on
// Linux. But it's unnecessary and needs to be avoided. Therefore a
// kSuccess is returned just for the sake of being invisible.
#define CHECK_MSTATUS_AND_RETURN_SILENT(status) \
    if (status != MStatus::kSuccess) return MStatus::kSuccess;

// struct to store the deformers when pick using D key
struct drawingDeformers {
    MMatrix mat;
    MPoint center;
    MPoint minPt;
    MPoint maxPt;
    MVector up, right;
    double width, height, depth;
};

// ---------------------------------------------------------------------
// the tool
// ---------------------------------------------------------------------

class skinBrushTool : public MPxToolCommand {
   public:
    skinBrushTool();
    ~skinBrushTool();

    static void *creator();
    static MSyntax newSyntax();

    MStatus parseArgs(const MArgList &args);

    MStatus doIt(const MArgList &args);
    MStatus redoIt();
    MStatus undoIt();
    MStatus setWeights(bool isUndo);
    MStatus callBrushRefresh();
    MStatus finalize();

    bool isUndoable() const;

    // setting the attributes
    void setColor(MColor color);
    void setCurve(int value);
    void setDrawBrush(bool value);
    void setDrawRange(bool value);
    void setPythonImportPath(MString value);
    void setEnterToolCommand(MString value);
    void setExitToolCommand(MString value);
    void setFractionOversampling(bool value);
    void setIgnoreLock(bool value);
    void setLineWidth(int value);
    void setMessage(int value);
    void setOversampling(int value);
    void setRange(double value);
    void setSize(double value);
    void setStrength(double value);
    void setSmoothStrength(double value);
    void setPruneWeights(double value);
    void setUndersampling(int value);
    void setVolume(bool value);
    void setCommandIndex(ModifierCommands value);
    void setSmoothRepeat(int value);
    void setSoloColor(int value);
    void setSoloColorType(int value);
    void setCoverage(bool value);
    void setPostSetting(bool value);

    void setInfluenceIndices(MIntArray indices);
    void setInfluenceName(MString name);
    void setMesh(MDagPath dagPath);
    void setNurbs(MDagPath dagPath);
    void setNormalize(bool value);

    void setSkinCluster(MObject skinCluster);
    void setIsNurbs(bool value);
    void setnumCVInV(int value);

    void setSkinClusterName(MString skinClusterName);
    MStatus getSkinClusterObj();

    void setWeights(MDoubleArray weights);
    void setUnoVertices(MIntArray editVertsIndices);
    void setUnoLocks(MIntArray locks);
    void setRedoLocks(MIntArray locks);

    void setMirrorTolerance(double value);
    void setPaintMirror(int value);
    void setUseColorSetsWhilePainting(bool value);
    void setDrawTriangles(bool value);
    void setDrawEdges(bool value);
    void setDrawPoints(bool value);
    void setDrawTransparency(bool value);

    void setMinColor(double value);
    void setMaxColor(double value);

   private:
    MColor colorVal;
    int curveVal;
    bool drawBrushVal;
    bool drawRangeVal;

    MString moduleImportString;
    MString enterToolCommandVal;
    MString exitToolCommandVal;
    bool fractionOversamplingVal;
    bool ignoreLockVal;
    int lineWidthVal;
    int messageVal;
    int oversamplingVal;
    double rangeVal;
    double sizeVal;
    double strengthVal, smoothStrengthVal;
    double pruneWeights;
    int undersamplingVal;
    bool volumeVal;

    bool coverageVal;
    int influenceIndex = 0;
    ModifierCommands commandIndex = ModifierCommands::Add;
    int smoothRepeat = 3;
    int soloColorTypeVal = 1;  // 1 lava
    int soloColorVal = 0;
    bool postSetting = true;

    int paintMirror = 0;  // intValue
    double mirrorMinDist = 0.05;
    bool useColorSetsWhilePainting = false;
    bool drawTriangles = true;
    bool drawEdges = true;
    bool drawPoints = true;
    bool drawTransparency = true;

    double minSoloColor = 0.0;
    double maxSoloColor = 1.0;

    MIntArray influenceIndices;
    MDagPath meshDag, nurbsDag;
    MObject skinObj;
    MString skinName;
    bool isNurbs = false;
    int numCVsInV_ = 0;

    bool normalize;
    MString influenceName;
    MDoubleArray redoWeights;
    MDoubleArray undoWeights;
    MIntArray undoVertices;
    MObject vertexComponents;

    MIntArray undoLocks, redoLocks;
};

// ---------------------------------------------------------------------
// the context
// ---------------------------------------------------------------------
class SkinBrushContext : public MPxContext {
   public:
    SkinBrushContext();
    void toolOnSetup(MEvent &event);
    void toolOffCleanup();

    void getClassName(MString &name) const;

    MStatus doPress(MEvent &event);
    MStatus doDrag(MEvent &event);
    MStatus doRelease(MEvent &event);

    void drawCircle(MPoint point, MMatrix mat, double radius);

    // VP2.0
    MStatus doPress(MEvent &event, MHWRender::MUIDrawManager &drawManager,
                    const MHWRender::MFrameContext &context);
    MStatus doDrag(MEvent &event, MHWRender::MUIDrawManager &drawManager,
                   const MHWRender::MFrameContext &context);
    MStatus doRelease(MEvent &event, MHWRender::MUIDrawManager &drawManager,
                      const MHWRender::MFrameContext &context);
    MStatus drawTheMesh(MHWRender::MUIDrawManager &drawManager, MVector worldVector);
    MStatus drawMeshWhileDrag(MHWRender::MUIDrawManager &drawManager);

    MStatus doPtrMoved(MEvent &event, MHWRender::MUIDrawManager &drawManager,
                       const MHWRender::MFrameContext &context);
    int getHighestInfluence(int faceHit, MFloatPoint hitPoint);
    int getClosestInfluenceToCursor(int screenX, int screenY);
    // common methods
    MStatus doPressCommon(MEvent event);
    // doDragCommon where the magic happens
    MStatus doDragCommon(MEvent event);
    MStatus doReleaseCommon(MEvent event);
    void doTheAction();
    ModifierCommands getCommandIndexModifiers();
    MStatus getMesh();
    MStatus getTheOrigMeshForMirror();

    void getConnectedVertices();
    void getConnectedVerticesSecond();
    void getConnectedVerticesThird();
    void getConnectedVerticesTyler();
    void getConnectedVerticesFlatten();
    std::vector<int> getSurroundingVerticesPerVert(int vertexIndex);
    std::vector<int> getSurroundingVerticesPerFace(int vertexIndex);

    void getFromMeshNormals();
    MStatus getSelection(MDagPath &dagPath);
    MStatus getSkinCluster(MDagPath meshDag, MObject &skinClusterObj);
    void refreshJointsLocks();
    void refresh();
    void refreshDeformerColor(int influenceInd);
    void refreshTheseVertices(MIntArray verticesIndices);
    void refreshMirrorInfluences(MIntArray inputMirrorInfluences);

    void mergeMirrorArray(std::unordered_map<int, float> &valuesBase,
                          std::unordered_map<int, float> &valuesMirrored);
    MStatus applyCommand(int influence, std::unordered_map<int, float> &valuesToSet);
    MStatus applyCommandMirror();
    MStatus refreshColors(MIntArray &editVertsIndices, MColorArray &multiEditColors,
                          MColorArray &soloEditColors);
    MStatus editSoloColorSet(bool doBlack);
    MColor getASoloColor(double val);
    MStatus refreshPointsNormals();

    void setColor(int vertexIndex, float value, MIntArray &editVertsIndices,
                  MColorArray &multiEditColors, MColorArray &soloEditColors,
                  bool useMirror = false);

    void setColorWithMirror(int vertexIndex, float valueBase, float valueMirror,
                            MIntArray &editVertsIndices, MColorArray &multiEditColors,
                            MColorArray &soloEditColors);

    MStatus querySkinClusterValues(MObject skinCluster, MIntArray &verticesIndices, bool doColors);
    MStatus fillArrayValues(MObject skinCluster, bool doColors);
    MStatus displayWeightValue(int vertexIndex, bool displayZero = false);
    MStatus fillArrayValuesDEP(MObject skinCluster, bool doColors);
    void getSkinClusterAttributes(MObject skinCluster, unsigned int &maxInfluences,
                                  bool &maintainMaxInfluences, unsigned int &normalize);
    MIntArray getInfluenceIndices();
    bool getMirrorHit(bool getNormal, int &faceHit, MFloatPoint &hitPoint);
    bool computeHit(short screenPixelX, short screenPixelY, bool getNormal, int &faceHit,
                    MFloatPoint &hitPoint);
    bool expandHit(int faceHit, MFloatPoint hitPoint, std::unordered_map<int, float> &dicVertsDist);

    void growArrayOfHitsFromCenters(std::unordered_map<int, float> &dicVertsDist,
                                    MFloatPointArray &AllHitPoints);

    // smooth computation
    MStatus preparePaint(std::unordered_map<int, float> &dicVertsDist,
                         std::unordered_map<int, float> &dicVertsDistPrevPaint,
                         std::vector<float> &intensityValues,
                         std::unordered_map<int, float> &skinValToSet, bool mirror);

    MStatus doPerformPaint();

    void addBrushShapeFallof(std::unordered_map<int, float> &dicVertsDist);

    MObject allVertexComponents();
    MIntArray getVerticesInVolume();
    void getVerticesInVolumeRange(int index, MIntArray volumeIndices, MIntArray &rangeIndices,
                                  MFloatArray &values);

    double getFalloffValue(double value, double strength);
    bool eventIsValid(MEvent event);

    void setInViewMessage(bool display);

    // setting the attributes
    void setColorR(float value);
    void setColorG(float value);
    void setColorB(float value);
    void setCurve(int value);
    void setDrawBrush(bool value);
    void setDrawRange(bool value);
    void setPythonImportPath(MString value);
    void setEnterToolCommand(MString value);
    void setExitToolCommand(MString value);
    void setFlood();
    void setVerbose(bool value);
    void setPickMaxInfluence(bool value);
    void setPickInfluence(bool value);
    void setFractionOversampling(bool value);
    void setIgnoreLock(bool value);
    void setLineWidth(int value);
    void setMessage(int value);
    void setOversampling(int value);
    void setRange(double value);
    void setSize(double value);
    void setStrength(double value);
    void setSmoothStrength(double value);
    void setUndersampling(int value);
    void setVolume(bool value);
    void setMirrorTolerance(double value);
    void setPaintMirror(int value);
    void setUseColorSetsWhilePainting(bool value);
    void setDrawTriangles(bool value);
    void setDrawEdges(bool value);
    void setDrawPoints(bool value);
    void setDrawTransparency(bool value);
    void setCoverage(bool value);
    void setInfluenceIndex(int value, bool selectInUI);
    void setCommandIndex(ModifierCommands value);
    void setSmoothRepeat(int value);
    void setSoloColor(int value);
    void maya2019RefreshColors(bool toggle = true);
    void setSoloColorType(int value);
    void setInfluenceByName(MString value);
    void setPostSetting(bool value);

    void setSkinClusterByName(MString& value);
    void setMeshByName(MString& value);

    void setMinColor(double value);
    void setMaxColor(double value);

    void setPruneWeights(double value);
    void setInteractiveValue(double value, int ind);

    // getting the attributes
    float getColorR();
    float getColorG();
    float getColorB();
    int getCurve();
    bool getDrawBrush();
    bool getDrawRange();
    MString getPythonImportPath();
    MString getEnterToolCommand();
    MString getExitToolCommand();
    bool getFractionOversampling();
    bool getIgnoreLock();

    int getLineWidth();
    int getMessage();
    int getOversampling();
    double getRange();
    double getSize();
    double getStrength();
    double getSmoothStrength();
    double getPruneWeights();
    double getInteractiveValue(int ind);
    int getUndersampling();
    bool getVolume();
    bool getCoverage();
    int getInfluenceIndex();
    MString getInfluenceName();
    MString getSkinClusterName();
    MString getMeshName();
    ModifierCommands getCommandIndex();
    int getSmoothRepeat();
    int getSoloColor();

    double getMirrorTolerance();
    int getPaintMirror();
    bool getUseColorSetsWhilePainting();
    bool getDrawTriangles();
    bool getDrawEdges();
    bool getDrawPoints();
    bool getDrawTransparency();
    int getSoloColorType();
    bool getPostSetting();
    double getMinColor();
    double getMaxColor();

   private:
    bool verbose = false;
    double interactiveValue = 1.0;   // for whateverUse in the code
    double interactiveValue1 = 1.0;  // for whateverUse in the code
    double interactiveValue2 = 1.0;  // for whateverUse in the code

    skinBrushTool *cmd;
    bool getSkinFromName = false;
    bool getMeshFromName = false;
    MString passedSkinName, passedMeshName;

    bool firstPaintDone;
    bool performBrush;
    int performRefreshViewPort;
    int maxRefreshValue = 2;
    int undersamplingSteps;

    int paintMirror = 0;
    double mirrorMinDist = 0.05;
    bool useColorSetsWhilePainting = false;

    bool drawTriangles = true;
    bool drawPoints = false;
    bool drawEdges = true;
    bool drawTransparency = true;

    // the tool settings
    MColor colorVal = MColor(1.0, 0, 0);
    int curveVal;
    bool drawBrushVal;
    bool drawRangeVal;

    MString moduleImportString;
    MString enterToolCommandVal;
    MString exitToolCommandVal;
    bool fractionOversamplingVal;
    bool ignoreLockVal;

    int lineWidthVal;
    int messageVal;
    int oversamplingVal;
    double rangeVal;
    double sizeVal;
    double strengthVal, smoothStrengthVal;
    bool shiftMiddleDrag = false;  // for the mniddleClick drag
    double storedDistance = 0.0;   // for the mniddleClick drag

    int undersamplingVal;
    bool volumeVal;
    bool coverageVal;
    // if we're asking to pick max influence
    bool pickMaxInfluenceVal = false, pickInfluenceVal = false;
    MString orderedIndicesByWeights;
    // for me yep ----
    int influenceIndex = 0, smoothRepeat = 4;
    ModifierCommands commandIndex = ModifierCommands::Add;

    int soloColorTypeVal = 1, soloColorVal = 0;  // 1 lava
    bool postSetting = true;                     // we apply paint as ssons as attr is changed
    bool doNormalize = true;

    // brush settings for adjusting
    bool initAdjust;                 // True after the first drag event.
                                     // Controls the adjust direction for
                                     // the size and the strength.
    MFloatPoint surfacePointAdjust;  // Initital surface point of the press
                                     // event.
    MVector worldVectorAdjust;       // Initial view vector of the press
                                     // event.
    bool sizeAdjust;                 // True, if the size is set.
    double adjustValue;              // The new value for the size or
                                     // strength.

    M3dView view;
    unsigned int width;
    unsigned int height;
    short viewCenterX;
    short viewCenterY;

    // the cursor position
    short screenX;
    short screenY;
    short startScreenX;
    short startScreenY;

    MPointArray surfacePoints;  // The cursor positions on the mesh in
                                // world space.
    // the worldPosition
    MPoint worldPoint;
    MPoint worldMirrorPoint;
    MVector worldVector;           // The view vector from the camera to
                                   // the surface point.
    MVector normalVector;          // The normal vector to camera
    MVector normalMirroredVector;  // The mirrored normal vector to camera

    MFloatPoint centerOfBrush;        // store the center of the bursh to display
    MFloatPoint centerOfMirrorBrush;  // store the center of the bursh to display

    MFloatPoint inMatrixHit;        // store the center of the bursh to display
    MFloatPoint inMatrixHitMirror;  // store the center of the bursh to display

    float pressDistance;  // The closest distance to the mesh on
    MStatus pressStatus;

    MFnMesh meshFn, meshOrigFn;
    MFnNurbsSurface nurbsFn;
    bool isNurbs = false;

    MFloatMatrix inclusiveMatrix, inclusiveMatrixInverse;
    MDagPath meshDag, nurbsDag;
    MDagPath origMeshDag;
    unsigned int numVertices = 0, numFaces = 0;

    unsigned int numElements = 0;
    unsigned int numCVsInV_ = 0, numCVsInU_ = 0;
    bool UIsPeriodic_ = false, VIsPeriodic_ = false;
    unsigned int UDeg_ = 0, VDeg_ = 0;

    MIntArray vtxSelection;  // The currently selected vertices. This
                             // is used for flooding.

    MObject attrValue;
    MDoubleArray valuesForAttribute, paintArrayValues;  // the array of values to paint

    MMeshIntersector intersectorOrigShape;
    MMeshIntersector intersector;

    std::vector<bool> selectedIndices;

    MObject allVtxCompObj;

    // the skin cluster
    MObject skinObj;
    unsigned int influenceCount;
    MIntArray influenceIndices;
    MDagPathArray inflDagPaths;
    std::vector<drawingDeformers> BBoxOfDeformers;

    MStringArray inflNames;
    MIntArray inflNamePixelSize;
    bool maintainMaxInfluences;
    unsigned int maxInfluences;
    bool normalize;

    MSelectionList prevSelection;
    MSelectionList prevHilite;

    // guillaume values ----------
    MMeshIsectAccelParams accelParams;
    MMeshIsectAccelParams accelParamsOrigMesh;
    bool foundBlurSkinAttribute = false;

    // skinCluster values --------------------------
    double pruneWeight;
    int nbJoints = 0, nbJointsBig = 0;
    MIntArray deformersIndices;
    MIntArray cpIds;  // the ids of the vertices passed as to update skin for
    std::vector<std::vector<std::pair<int, float>>> skin_weights_;
    MDoubleArray skinWeightList, fullUndoSkinWeightList, skinWeightsForUndo;
    MIntArray indicesForInfluenceObjects;  // on skinCluster for sparse array

    // mirror things -----
    MIntArray mirrorInfluences;  // indices of the mirror influences

    std::vector<bool> influenceLocks;
    MIntArray lockJoints, ignoreLockJoints, lockVertices;

    // colorSet ------------------------
    MDGModifier colorSetMod;
    MDoubleArray soloColorsValues;
    MColor lockVertColor = MColor((float)0.2, (float)0.2, (float)0.2);
    MColor lockJntColor = MColor((float)0.2, (float)0.2, (float)0.2);
    MString fullColorSet = MString("multiColorsSet");
    MString soloColorSet = MString("soloColorsSet");
    MString fullColorSet2 = MString("multiColorsSet2");
    MString soloColorSet2 = MString("soloColorsSet2");

    bool toggleColorState = false;  // use to swap from colorSet and colorSet2

    double minSoloColor = 0.0;
    double maxSoloColor = 1.0;

    MColorArray multiCurrentColors, jointsColors,
        soloCurrentColors;  // lock vertices color are not stored inside these arrays

    MIntArray VertexCountPerPolygon, fullVertexList;
    std::vector<MIntArray> perVertexFaces;             // per vertex Faces
    std::vector<MIntArray> perFaceVertices;            // per face vertices
    std::vector<MIntArray> perVertexEdges;             // per face vertices
    std::vector<std::pair<int, int>> perEdgeVertices;  // to draw the wireframe
    std::vector<std::vector<MIntArray>> perFaceTriangleVertices;

    std::vector<std::vector<int>> perVertexVerticesSet;  // per vertex vertices
    std::vector<std::vector<int>> perFaceVerticesSet;    // per Face Vertices
    std::vector<std::vector<int>> normalsIds;            // vector of faces Ids normals

    // Try the Flat Version
    std::vector<int> perVertexVerticesSetFLAT;
    std::vector<int> perVertexVerticesSetINDEX;

    std::vector<int> perFaceVerticesSetFLAT;
    std::vector<int> perFaceVerticesSetINDEX;

    MVectorArray verticesNormals;
    MIntArray verticesNormalsIndices;

    const float *rawNormals;
    const float *mayaRawPoints;
    const float *mayaOrigRawPoints;
    MFloatPoint origHitPoint;

    MPointArray meshPoints;
    int fullVertexListLength = 0;

    // HITs vairables ------------------------
    bool successFullHit = false;
    bool successFullMirrorHit = false;  // need to transfer this info to doDragCommon I believe
    bool successFullDragHit = false;
    bool successFullDragMirrorHit = false;
    bool refreshDone = false;

    MFloatPointArray AllHitPoints, AllHitPointsMirror;

    std::unordered_map<int, float> dicVertsDistSTART, previousPaint;
    std::unordered_map<int, float> dicVertsMirrorDistSTART, previousMirrorPaint;
    std::unordered_map<int, float> skinValuesToSet;
    std::unordered_map<int, float> skinValuesMirrorToSet;
    std::set<int> verticesPainted;  // the vertices that have been painted for a redraw purpose

    std::unordered_map<int, std::pair<float, float>> mirroredJoinedArray;
    std::vector<float> intensityValuesOrig;    // (length, 0);
    std::vector<float> intensityValuesMirror;  // (length, 0);

    ModifierKeys modifierNoneShiftControl = ModifierKeys::NoModifier;  // store the modifier type

    int previousfaceHit;   // the faceIndex that was hit during the press common
    int biggestInfluence;  // for while we search for biggest influence
};

// ---------------------------------------------------------------------
// command to create the context
// ---------------------------------------------------------------------

class SkinBrushContextCmd : public MPxContextCommand {
   public:
    SkinBrushContextCmd();
    MPxContext *makeObj();
    static void *creator();
    MStatus appendSyntax();
    MStatus doEditFlags();
    MStatus doQueryFlags();

   protected:
    SkinBrushContext *smoothContext;
};

#endif
