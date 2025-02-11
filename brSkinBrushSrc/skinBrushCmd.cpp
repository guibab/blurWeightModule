#include "skinBrushFlags.h"
#include "skinBrushTool.h"

// ---------------------------------------------------------------------
// command to create the context
// ---------------------------------------------------------------------
SkinBrushContextCmd::SkinBrushContextCmd() {
    // MGlobal::displayInfo(MString("---------------- [SkinBrushContextCmd()]------------------"));
}

MPxContext* SkinBrushContextCmd::makeObj() {
    // MGlobal::displayInfo(MString("---------------- [makeObj()]------------------"));
    smoothContext = new SkinBrushContext();
    return smoothContext;
}

void* SkinBrushContextCmd::creator() {
    // MGlobal::displayInfo(MString("---------------- [creator()]------------------"));
    return new SkinBrushContextCmd();
}
// ---------------------------------------------------------------------
// pointers for the argument flags
// ---------------------------------------------------------------------

MStatus SkinBrushContextCmd::appendSyntax() {
    MSyntax syn = syntax();

    syn.addFlag(kColorRFlag, kColorRFlagLong, MSyntax::kDouble);
    syn.addFlag(kColorGFlag, kColorGFlagLong, MSyntax::kDouble);
    syn.addFlag(kColorBFlag, kColorBFlagLong, MSyntax::kDouble);
    syn.addFlag(kCurveFlag, kCurveFlagLong, MSyntax::kLong);
    syn.addFlag(kDrawBrushFlag, kDrawBrushFlagLong, MSyntax::kBoolean);
    syn.addFlag(kDrawRangeFlag, kDrawRangeFlagLong, MSyntax::kBoolean);
    syn.addFlag(kImportPythonFlag, kImportPythonFlagLong, MSyntax::kString);
    syn.addFlag(kEnterToolCommandFlag, kEnterToolCommandFlagLong, MSyntax::kString);
    syn.addFlag(kExitToolCommandFlag, kExitToolCommandFlagLong, MSyntax::kString);

    syn.addFlag(kFloodFlag, kFloodFlagLong, MSyntax::kNoArg);
    syn.addFlag(kVerboseFlag, kVerboseFlagLong, MSyntax::kBoolean);

    syn.addFlag(kFractionOversamplingFlag, kFractionOversamplingFlagLong, MSyntax::kBoolean);
    syn.addFlag(kIgnoreLockFlag, kIgnoreLockFlagLong, MSyntax::kBoolean);
    syn.addFlag(kLineWidthFlag, kLineWidthFlagLong, MSyntax::kLong);
    syn.addFlag(kMessageFlag, kMessageFlagLong, MSyntax::kLong);
    syn.addFlag(kOversamplingFlag, kOversamplingFlagLong, MSyntax::kLong);
    syn.addFlag(kRangeFlag, kRangeFlagLong, MSyntax::kDouble);
    syn.addFlag(kSizeFlag, kSizeFlagLong, MSyntax::kDouble);
    syn.addFlag(kStrengthFlag, kStrengthFlagLong, MSyntax::kDouble);
    syn.addFlag(kUndersamplingFlag, kUndersamplingFlagLong, MSyntax::kLong);
    syn.addFlag(kVolumeFlag, kVolumeFlagLong, MSyntax::kBoolean);

    syn.addFlag(kSmoothStrengthFlag, kSmoothStrengthFlagLong, MSyntax::kDouble);

    syn.addFlag(kPruneWeightsFlag, kPruneWeightsFlagLong, MSyntax::kDouble);
    syn.addFlag(kCommandIndexFlag, kCommandIndexFlagLong, MSyntax::kLong);
    syn.addFlag(kSoloColorFlag, kSoloColorFlagLong, MSyntax::kLong);
    syn.addFlag(kSoloColorTypeFlag, kSoloColorTypeFlagLong, MSyntax::kLong);
    syn.addFlag(kCoverageFlag, kCoverageLong, MSyntax::kLong);
    syn.addFlag(kPickMaxInfluenceFlag, kPickMaxInfluenceFlagLong, MSyntax::kBoolean);

    syn.addFlag(kPickInfluenceFlag, kPickInfluenceFlagLong, MSyntax::kBoolean);
    syn.addFlag(kInfluenceIndexFlag, kInfluenceIndexFlagLong, MSyntax::kLong);
    syn.addFlag(kInfluenceNameFlag, kInfluenceNameFlagLong, MSyntax::kString);
    syn.addFlag(kPostSettingFlag, kPostSettingFlagLong, MSyntax::kBoolean);
    syn.addFlag(kRefreshFlag, kRefreshFlagLong, MSyntax::kBoolean);
    syn.addFlag(kRefreshPtsNmsFlag, kRefreshPtsNmsFlagLong, MSyntax::kBoolean);
    syn.addFlag(kRefreshLocksJointsFlag, kRefreshLocksJointsFlagLong, MSyntax::kBoolean);

    syn.addFlag(kRefreshDfmColorFlag, kRefreshDfmColorFlagLong, MSyntax::kLong);
    syn.addFlag(kSmoothRepeatFlag, kSmoothRepeatFlagLong, MSyntax::kLong);

    syn.addFlag(kSkinClusterNameFlag, kSkinClusterNameFlagLong, MSyntax::kString);
    syn.addFlag(kMeshNameFlag, kMeshNameFlagLong, MSyntax::kString);

    syn.addFlag(kPaintMirrorToleranceFlag, kPaintMirrorToleranceFlagLong, MSyntax::kDouble);
    syn.addFlag(kPaintMirrorFlag, kPaintMirrorFlagLong, MSyntax::kLong);
    syn.addFlag(kUseColorSetWhilePaintingFlag, kUseColorSetWhilePaintingFlagLong,
                MSyntax::kBoolean);
    syn.addFlag(kMeshDragDrawTrianglesFlag, kMeshDragDrawTrianglesFlagLong, MSyntax::kBoolean);
    syn.addFlag(kMeshDragDrawEdgesFlag, kMeshDragDrawEdgesFlagLong, MSyntax::kBoolean);
    syn.addFlag(kMeshDragDrawPointsFlag, kMeshDragDrawPointsFlagLong, MSyntax::kBoolean);
    syn.addFlag(kMeshDragDrawTransFlag, kMeshDragDrawTransFlagLong, MSyntax::kBoolean);

    syn.addFlag(kInteractiveValueFlag, kInteractiveValueFlagLong, MSyntax::kDouble);
    syn.addFlag(kInteractiveValue1Flag, kInteractiveValue1FlagLong, MSyntax::kDouble);
    syn.addFlag(kInteractiveValue2Flag, kInteractiveValue2FlagLong, MSyntax::kDouble);

    syn.addFlag(kMinColorFlag, kMinColorFlagLong, MSyntax::kDouble);
    syn.addFlag(kMaxColorFlag, kMaxColorFlagLong, MSyntax::kDouble);

    syn.addFlag(kListVerticesIndicesFlag, kListVerticesIndicesFlagLong, MSyntax::kLong);
    syn.makeFlagMultiUse(kListVerticesIndicesFlag);

    syn.addFlag(kMirrorInfluencesFlag, kMirrorInfluencesFlagLong, MSyntax::kLong);
    syn.makeFlagMultiUse(kMirrorInfluencesFlag);

    return MStatus::kSuccess;
}

MStatus SkinBrushContextCmd::doEditFlags() {
    MStatus status = MStatus::kSuccess;

    MArgParser argData = parser();

    if (argData.isFlagSet(kColorRFlag)) {
        double value;
        status = argData.getFlagArgument(kColorRFlag, 0, value);
        smoothContext->setColorR((float)value);
    }

    if (argData.isFlagSet(kColorGFlag)) {
        double value;
        status = argData.getFlagArgument(kColorGFlag, 0, value);
        smoothContext->setColorG((float)value);
    }

    if (argData.isFlagSet(kColorBFlag)) {
        double value;
        status = argData.getFlagArgument(kColorBFlag, 0, value);
        smoothContext->setColorB((float)value);
    }

    if (argData.isFlagSet(kCurveFlag)) {
        int value;
        status = argData.getFlagArgument(kCurveFlag, 0, value);
        smoothContext->setCurve(value);
    }

    if (argData.isFlagSet(kDrawBrushFlag)) {
        bool value;
        status = argData.getFlagArgument(kDrawBrushFlag, 0, value);
        smoothContext->setDrawBrush(value);
    }

    if (argData.isFlagSet(kDrawRangeFlag)) {
        bool value;
        status = argData.getFlagArgument(kDrawRangeFlag, 0, value);
        smoothContext->setDrawRange(value);
    }

    if (argData.isFlagSet(kImportPythonFlag)) {
        MString value;
        status = argData.getFlagArgument(kImportPythonFlag, 0, value);
        smoothContext->setPythonImportPath(value);
    }

    if (argData.isFlagSet(kEnterToolCommandFlag)) {
        MString value;
        status = argData.getFlagArgument(kEnterToolCommandFlag, 0, value);
        smoothContext->setEnterToolCommand(value);
    }

    if (argData.isFlagSet(kExitToolCommandFlag)) {
        MString value;
        status = argData.getFlagArgument(kExitToolCommandFlag, 0, value);
        smoothContext->setExitToolCommand(value);
    }

    if (argData.isFlagSet(kFloodFlag)) {
        smoothContext->setFlood();
    }
    if (argData.isFlagSet(kVerboseFlag)) {
        bool value;
        status = argData.getFlagArgument(kVerboseFlag, 0, value);
        smoothContext->setVerbose(value);
    }

    if (argData.isFlagSet(kRefreshFlag)) {
        smoothContext->refresh();
    }

    if (argData.isFlagSet(kRefreshDfmColorFlag)) {
        int value;
        status = argData.getFlagArgument(kRefreshDfmColorFlag, 0, value);
        smoothContext->refreshDeformerColor(value);
    }

    if (argData.isFlagSet(kRefreshPtsNmsFlag)) {
        smoothContext->refreshPointsNormals();
    }

    if (argData.isFlagSet(kRefreshLocksJointsFlag)) {
        smoothContext->refreshJointsLocks();
    }

    if (argData.isFlagSet(kListVerticesIndicesFlag)) {
        bool foundListVerticesIndices = true;
        MIntArray indicesVertices;
        int nbUse = argData.numberOfFlagUses(kListVerticesIndicesFlag);
        MString toDisplay("List Vertices : ");
        for (int i = 0; i < nbUse; i++) {
            MArgList flagArgs;
            argData.getFlagArgumentList(kListVerticesIndicesFlag, i, flagArgs);
            int vtxIndex;
            flagArgs.get(0, vtxIndex);
            indicesVertices.append(vtxIndex);
            toDisplay += vtxIndex;
            toDisplay += MString(" - ");
        }
        smoothContext->refreshTheseVertices(indicesVertices);
    }

    if (argData.isFlagSet(kMirrorInfluencesFlag)) {
        MIntArray mirrorInfluencesIndices;
        int nbUse = argData.numberOfFlagUses(kMirrorInfluencesFlag);
        for (int i = 0; i < nbUse; i++) {
            MArgList flagArgs;
            argData.getFlagArgumentList(kMirrorInfluencesFlag, i, flagArgs);
            int influenceIndex;
            flagArgs.get(0, influenceIndex);
            mirrorInfluencesIndices.append(influenceIndex);
        }
        smoothContext->refreshMirrorInfluences(mirrorInfluencesIndices);
    }

    if (argData.isFlagSet(kPickMaxInfluenceFlag)) {
        bool value;
        status = argData.getFlagArgument(kPickMaxInfluenceFlag, 0, value);
        smoothContext->setPickMaxInfluence(value);
    }
    if (argData.isFlagSet(kPickInfluenceFlag)) {
        bool value;
        status = argData.getFlagArgument(kPickInfluenceFlag, 0, value);
        smoothContext->setPickInfluence(value);
    }

    if (argData.isFlagSet(kInfluenceIndexFlag)) {
        int value;
        status = argData.getFlagArgument(kInfluenceIndexFlag, 0, value);
        smoothContext->setInfluenceIndex(value, false);  // not reselect in UI
    }
    if (argData.isFlagSet(kInfluenceNameFlag)) {
        MString value;
        status = argData.getFlagArgument(kInfluenceNameFlag, 0, value);
        smoothContext->setInfluenceByName(value);  // not reselect in UI
    }
    if (argData.isFlagSet(kSkinClusterNameFlag)) {
        MString value;
        status = argData.getFlagArgument(kSkinClusterNameFlag, 0, value);
        MGlobal::displayInfo(MString("kSkinClusterNameFlag passed ") + value);
        smoothContext->setSkinClusterByName(value);  // not reselect in UI
    }
    if (argData.isFlagSet(kMeshNameFlag)) {
        MString value;
        status = argData.getFlagArgument(kMeshNameFlag, 0, value);
        MGlobal::displayInfo(MString("kMeshNameFlag passed ") + value);
        smoothContext->setMeshByName(value);  // not reselect in UI
    }
    if (argData.isFlagSet(kPostSettingFlag)) {
        bool value;
        status = argData.getFlagArgument(kPostSettingFlag, 0, value);
        smoothContext->setPostSetting(value);
    }

    if (argData.isFlagSet(kFractionOversamplingFlag)) {
        bool value;
        status = argData.getFlagArgument(kFractionOversamplingFlag, 0, value);
        smoothContext->setFractionOversampling(value);
    }

    if (argData.isFlagSet(kIgnoreLockFlag)) {
        bool value;
        status = argData.getFlagArgument(kIgnoreLockFlag, 0, value);
        smoothContext->setIgnoreLock(value);
    }

    if (argData.isFlagSet(kLineWidthFlag)) {
        int value;
        status = argData.getFlagArgument(kLineWidthFlag, 0, value);
        smoothContext->setLineWidth(value);
    }

    if (argData.isFlagSet(kMessageFlag)) {
        int value;
        status = argData.getFlagArgument(kMessageFlag, 0, value);
        smoothContext->setMessage(value);
    }

    if (argData.isFlagSet(kOversamplingFlag)) {
        int value;
        status = argData.getFlagArgument(kOversamplingFlag, 0, value);
        smoothContext->setOversampling(value);
    }

    if (argData.isFlagSet(kRangeFlag)) {
        double value;
        status = argData.getFlagArgument(kRangeFlag, 0, value);
        smoothContext->setRange(value);
    }

    if (argData.isFlagSet(kSizeFlag)) {
        double value;
        status = argData.getFlagArgument(kSizeFlag, 0, value);
        smoothContext->setSize(value);
    }

    if (argData.isFlagSet(kStrengthFlag)) {
        double value;
        status = argData.getFlagArgument(kStrengthFlag, 0, value);
        smoothContext->setStrength(value);
    }

    if (argData.isFlagSet(kSmoothStrengthFlag)) {
        double value;
        status = argData.getFlagArgument(kSmoothStrengthFlag, 0, value);
        smoothContext->setSmoothStrength(value);
    }

    if (argData.isFlagSet(kPruneWeightsFlag)) {
        double value;
        status = argData.getFlagArgument(kPruneWeightsFlag, 0, value);
        smoothContext->setPruneWeights(value);
    }

    if (argData.isFlagSet(kInteractiveValueFlag)) {
        double value;
        status = argData.getFlagArgument(kInteractiveValueFlag, 0, value);
        smoothContext->setInteractiveValue(value, 0);
    }
    if (argData.isFlagSet(kInteractiveValue1Flag)) {
        double value;
        status = argData.getFlagArgument(kInteractiveValue1Flag, 0, value);
        smoothContext->setInteractiveValue(value, 1);
    }
    if (argData.isFlagSet(kInteractiveValue2Flag)) {
        double value;
        status = argData.getFlagArgument(kInteractiveValue2Flag, 0, value);
        smoothContext->setInteractiveValue(value, 2);
    }

    if (argData.isFlagSet(kUndersamplingFlag)) {
        int value;
        status = argData.getFlagArgument(kUndersamplingFlag, 0, value);
        smoothContext->setUndersampling(value);
    }

    if (argData.isFlagSet(kVolumeFlag)) {
        bool value;
        status = argData.getFlagArgument(kVolumeFlag, 0, value);
        smoothContext->setVolume(value);
    }

    if (argData.isFlagSet(kCommandIndexFlag)) {
        int value;
        status = argData.getFlagArgument(kCommandIndexFlag, 0, value);
        smoothContext->setCommandIndex(static_cast<ModifierCommands>(value));
    }

    if (argData.isFlagSet(kSmoothRepeatFlag)) {
        int value;
        status = argData.getFlagArgument(kSmoothRepeatFlag, 0, value);
        smoothContext->setSmoothRepeat(value);
    }

    if (argData.isFlagSet(kSoloColorFlag)) {
        int value;
        status = argData.getFlagArgument(kSoloColorFlag, 0, value);
        smoothContext->setSoloColor(value);
    }
    if (argData.isFlagSet(kSoloColorTypeFlag)) {
        int value;
        status = argData.getFlagArgument(kSoloColorTypeFlag, 0, value);
        smoothContext->setSoloColorType(value);
    }

    if (argData.isFlagSet(kCoverageFlag)) {
        bool value;
        status = argData.getFlagArgument(kCoverageFlag, 0, value);
        smoothContext->setCoverage(value);
    }

    if (argData.isFlagSet(kPaintMirrorToleranceFlag)) {
        double value;
        status = argData.getFlagArgument(kPaintMirrorToleranceFlag, 0, value);
        smoothContext->setMirrorTolerance(value);
    }

    if (argData.isFlagSet(kPaintMirrorFlag)) {
        int value;
        status = argData.getFlagArgument(kPaintMirrorFlag, 0, value);
        smoothContext->setPaintMirror(value);
    }

    if (argData.isFlagSet(kUseColorSetWhilePaintingFlag)) {
        bool value;
        status = argData.getFlagArgument(kUseColorSetWhilePaintingFlag, 0, value);
        smoothContext->setUseColorSetsWhilePainting(value);
    }

    if (argData.isFlagSet(kMeshDragDrawTrianglesFlag)) {
        bool value;
        status = argData.getFlagArgument(kMeshDragDrawTrianglesFlag, 0, value);
        smoothContext->setDrawTriangles(value);
    }

    if (argData.isFlagSet(kMeshDragDrawEdgesFlag)) {
        bool value;
        status = argData.getFlagArgument(kMeshDragDrawEdgesFlag, 0, value);
        smoothContext->setDrawEdges(value);
    }

    if (argData.isFlagSet(kMeshDragDrawPointsFlag)) {
        bool value;
        status = argData.getFlagArgument(kMeshDragDrawPointsFlag, 0, value);
        smoothContext->setDrawPoints(value);
    }

    if (argData.isFlagSet(kMeshDragDrawTransFlag)) {
        bool value;
        status = argData.getFlagArgument(kMeshDragDrawTransFlag, 0, value);
        smoothContext->setDrawTransparency(value);
    }

    if (argData.isFlagSet(kMinColorFlag)) {
        double value;
        status = argData.getFlagArgument(kMinColorFlag, 0, value);
        smoothContext->setMinColor(value);
    }

    if (argData.isFlagSet(kMaxColorFlag)) {
        double value;
        status = argData.getFlagArgument(kMaxColorFlag, 0, value);
        smoothContext->setMaxColor(value);
    }

    return status;
}

MStatus SkinBrushContextCmd::doQueryFlags() {
    MArgParser argData = parser();

    if (argData.isFlagSet(kColorRFlag)) setResult(smoothContext->getColorR());

    if (argData.isFlagSet(kColorGFlag)) setResult(smoothContext->getColorG());

    if (argData.isFlagSet(kColorBFlag)) setResult(smoothContext->getColorB());

    if (argData.isFlagSet(kCurveFlag)) setResult(smoothContext->getCurve());

    if (argData.isFlagSet(kDrawBrushFlag)) setResult(smoothContext->getDrawBrush());

    if (argData.isFlagSet(kDrawRangeFlag)) setResult(smoothContext->getDrawRange());

    if (argData.isFlagSet(kImportPythonFlag)) setResult(smoothContext->getPythonImportPath());

    if (argData.isFlagSet(kEnterToolCommandFlag)) setResult(smoothContext->getEnterToolCommand());

    if (argData.isFlagSet(kExitToolCommandFlag)) setResult(smoothContext->getExitToolCommand());

    if (argData.isFlagSet(kFractionOversamplingFlag))
        setResult(smoothContext->getFractionOversampling());

    if (argData.isFlagSet(kIgnoreLockFlag)) setResult(smoothContext->getIgnoreLock());

    if (argData.isFlagSet(kLineWidthFlag)) setResult(smoothContext->getLineWidth());

    if (argData.isFlagSet(kMessageFlag)) setResult(smoothContext->getMessage());

    if (argData.isFlagSet(kOversamplingFlag)) setResult(smoothContext->getOversampling());

    if (argData.isFlagSet(kRangeFlag)) setResult(smoothContext->getRange());

    if (argData.isFlagSet(kSizeFlag)) setResult(smoothContext->getSize());

    if (argData.isFlagSet(kStrengthFlag)) setResult(smoothContext->getStrength());

    if (argData.isFlagSet(kSmoothStrengthFlag)) setResult(smoothContext->getSmoothStrength());

    if (argData.isFlagSet(kPruneWeightsFlag)) setResult(smoothContext->getPruneWeights());

    if (argData.isFlagSet(kInteractiveValueFlag)) setResult(smoothContext->getInteractiveValue(0));
    if (argData.isFlagSet(kInteractiveValue1Flag)) setResult(smoothContext->getInteractiveValue(1));
    if (argData.isFlagSet(kInteractiveValue2Flag)) setResult(smoothContext->getInteractiveValue(2));

    if (argData.isFlagSet(kUndersamplingFlag)) setResult(smoothContext->getUndersampling());

    if (argData.isFlagSet(kVolumeFlag)) setResult(smoothContext->getVolume());

    if (argData.isFlagSet(kCoverageFlag)) setResult(smoothContext->getCoverage());

    if (argData.isFlagSet(kInfluenceIndexFlag)) setResult(smoothContext->getInfluenceIndex());

    if (argData.isFlagSet(kInfluenceNameFlag)) setResult(smoothContext->getInfluenceName());

    if (argData.isFlagSet(kSkinClusterNameFlag)) setResult(smoothContext->getSkinClusterName());

    if (argData.isFlagSet(kMeshNameFlag)) setResult(smoothContext->getMeshName());

    if (argData.isFlagSet(kCommandIndexFlag)) setResult(static_cast<int>(smoothContext->getCommandIndex()));

    if (argData.isFlagSet(kSmoothRepeatFlag)) setResult(smoothContext->getSmoothRepeat());

    if (argData.isFlagSet(kSoloColorFlag)) setResult(smoothContext->getSoloColor());

    if (argData.isFlagSet(kSoloColorTypeFlag)) setResult(smoothContext->getSoloColorType());

    if (argData.isFlagSet(kPaintMirrorToleranceFlag))
        setResult(smoothContext->getMirrorTolerance());

    if (argData.isFlagSet(kPaintMirrorFlag)) setResult(smoothContext->getPaintMirror());

    if (argData.isFlagSet(kUseColorSetWhilePaintingFlag))
        setResult(smoothContext->getUseColorSetsWhilePainting());

    if (argData.isFlagSet(kMeshDragDrawTrianglesFlag)) setResult(smoothContext->getDrawTriangles());

    if (argData.isFlagSet(kMeshDragDrawEdgesFlag)) setResult(smoothContext->getDrawEdges());

    if (argData.isFlagSet(kMeshDragDrawPointsFlag)) setResult(smoothContext->getDrawPoints());

    if (argData.isFlagSet(kMeshDragDrawTransFlag)) setResult(smoothContext->getDrawTransparency());

    if (argData.isFlagSet(kPostSettingFlag)) setResult(smoothContext->getPostSetting());

    if (argData.isFlagSet(kMinColorFlag)) setResult(smoothContext->getMinColor());

    if (argData.isFlagSet(kMaxColorFlag)) setResult(smoothContext->getMaxColor());

    return MStatus::kSuccess;
}
