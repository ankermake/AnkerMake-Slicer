set(SLIC3R_APP_NAME "slic3r")
set(SLIC3R_APP_KEY "slic3r")
set(SLIC3R_VERSION "0.0.0")
set(SLIC3R_BUILD_ID "0.0.0 alpha")

set(CMAKE_CXX_STANDARD 17)

if (MINGW)
    add_compile_options(-Wa,-mbig-obj)
endif ()

set(SRCS
    BoundingBox.cpp
    BoundingBox.hpp
    #BridgeDetector.cpp
    #BridgeDetector.hpp
    ClipperUtils.cpp
    ClipperUtils.hpp
    Config.cpp
    Config.hpp
    #EdgeGrid.cpp
    #EdgeGrid.hpp
    #ElephantFootCompensation.cpp
    #ElephantFootCompensation.hpp
    enum_bitmask.hpp
    ExPolygon.cpp
    ExPolygon.hpp
    ExPolygonCollection.cpp
    ExPolygonCollection.hpp
    #Extruder.cpp
    #Extruder.hpp
    ExtrusionEntity.cpp
    ExtrusionEntity.hpp
    #ExtrusionEntityCollection.cpp
    #ExtrusionEntityCollection.hpp
    #ExtrusionSimulator.cpp
    #ExtrusionSimulator.hpp
    #FileParserError.hpp
    #Fill/Fill.cpp
    #Fill/Fill.hpp
    #Fill/Fill3DHoneycomb.cpp
    #Fill/Fill3DHoneycomb.hpp
    #Fill/FillAdaptive.cpp
    #Fill/FillAdaptive.hpp
    #Fill/FillBase.cpp
    #Fill/FillBase.hpp
    #Fill/FillConcentric.cpp
    #Fill/FillConcentric.hpp
    #Fill/FillHoneycomb.cpp
    #Fill/FillHoneycomb.hpp
    #Fill/FillGyroid.cpp
    #Fill/FillGyroid.hpp
    #Fill/FillPlanePath.cpp
    #Fill/FillPlanePath.hpp
    #Fill/FillLine.cpp
    #Fill/FillLine.hpp
    #Fill/FillRectilinear.cpp
    #Fill/FillRectilinear.hpp
    Flow.cpp
    Flow.hpp
    format.hpp
    #Format/3mf.cpp
    #Format/3mf.hpp
    #Format/AMF.cpp
    #Format/AMF.hpp
    #Format/OBJ.cpp
    #Format/OBJ.hpp
    Format/objparser.cpp
    Format/objparser.hpp
    #Format/PRUS.cpp
    #Format/PRUS.hpp
    #Format/STL.cpp
    #Format/STL.hpp
    #Format/SL1.hpp
    #Format/SL1.cpp
    #GCode/ThumbnailData.cpp
    #GCode/ThumbnailData.hpp
    #GCode/CoolingBuffer.cpp
    #GCode/CoolingBuffer.hpp
    #GCode/PostProcessor.cpp
    #GCode/PostProcessor.hpp
    #GCode/PressureEqualizer.cpp
    #GCode/PressureEqualizer.hpp
    #GCode/PrintExtents.cpp
    #GCode/PrintExtents.hpp
    #GCode/SpiralVase.cpp
    #GCode/SpiralVase.hpp
    #GCode/SeamPlacer.cpp
    #GCode/SeamPlacer.hpp
    #GCode/ToolOrdering.cpp
    #GCode/ToolOrdering.hpp
    #GCode/WipeTower.cpp
    #GCode/WipeTower.hpp
    #GCode/GCodeProcessor.cpp
    #GCode/GCodeProcessor.hpp
    #GCode.cpp
    #GCode.hpp
    #GCodeReader.cpp
    #GCodeReader.hpp
    #GCodeWriter.cpp
    #GCodeWriter.hpp
    Geometry.cpp
    Geometry.hpp
    #KDTreeIndirect.hpp
    #Layer.cpp
    #Layer.hpp
    #LayerRegion.cpp
    libslic3r.h
    libslic3r_version.h
    Line.cpp
    Line.hpp
    #Model.cpp
    #Model.hpp
    #ModelArrange.hpp
    #ModelArrange.cpp
    #CustomGCode.cpp
    #CustomGCode.hpp
    #Arrange.hpp
    #Arrange.cpp
    #MotionPlanner.cpp
    #MotionPlanner.hpp
    MultiPoint.cpp
    MultiPoint.hpp
    #MutablePriorityQueue.hpp
    #ObjectID.cpp
    #ObjectID.hpp
    #PerimeterGenerator.cpp
    #PerimeterGenerator.hpp
    #PlaceholderParser.cpp
    #PlaceholderParser.hpp
    #Platform.cpp
    #Platform.hpp
    Point.cpp
    Point.hpp
    Polygon.cpp
    Polygon.hpp
    #PolygonTrimmer.cpp
    #PolygonTrimmer.hpp
    Polyline.cpp
    Polyline.hpp
	#clipper.hpp
	#clipper.cpp
    #Preset.cpp
    #Preset.hpp
    #PresetBundle.cpp
    #PresetBundle.hpp
    #AppConfig.cpp
    #AppConfig.hpp
    #Print.cpp
    #Print.hpp
    #PrintBase.cpp
    #PrintBase.hpp
    PrintConfig.cpp
    PrintConfig.hpp
    #PrintObject.cpp
    #PrintRegion.cpp
    #PNGRead.hpp
    #PNGRead.cpp
    #Semver.cpp
    ShortestPath.cpp
    ShortestPath.hpp
    #SLAPrint.cpp
    #SLAPrintSteps.cpp
    #SLAPrintSteps.hpp
    #SLAPrint.hpp
    #Slicing.cpp
    #Slicing.hpp
    #SlicesToTriangleMesh.hpp
    #SlicesToTriangleMesh.cpp
    #SlicingAdaptive.cpp
    #SlicingAdaptive.hpp
    #SupportMaterial.cpp
    #SupportMaterial.hpp
    #Surface.cpp
    #Surface.hpp
    #SurfaceCollection.cpp
    #SurfaceCollection.hpp
    SVG.cpp
    SVG.hpp
    #Technologies.hpp
    Tesselate.cpp
    Tesselate.hpp
    TriangleMesh.cpp
    TriangleMesh.hpp
    TriangulateWall.hpp
    TriangulateWall.cpp
    utils.cpp
    Utils.hpp
    Time.cpp
    Time.hpp
    #Thread.cpp
    #Thread.hpp
    #TriangleSelector.cpp
    #TriangleSelector.hpp
    #MTUtils.hpp
    #VoronoiOffset.cpp
    #VoronoiOffset.hpp
    #VoronoiVisualUtils.hpp
    #Zipper.hpp
    #Zipper.cpp
    #MinAreaBoundingBox.hpp
    #MinAreaBoundingBox.cpp
    #miniz_extension.hpp
    #miniz_extension.cpp
    #SimplifyMesh.hpp
    #SimplifyMeshImpl.hpp
    #SimplifyMesh.cpp
    #MarchingSquares.hpp
    #Optimize/Optimizer.hpp
    #Optimize/NLoptOptimizer.hpp
    #Optimize/BruteforceOptimizer.hpp
    SLA/Pad.hpp
    SLA/Pad.cpp
    SLA/SupportTreeBuilder.hpp
	SLA/SupportTreeBuilder.cpp
    SLA/SupportTreeMesher.hpp
    SLA/SupportTreeMesher.cpp
    SLA/SupportTreeBuildsteps.hpp
    SLA/SupportTreeBuildsteps.cpp
    SLA/Concurrency.hpp
#    SLA/SupportTree.hpp
#    SLA/SupportTree.cpp
#    SLA/SupportTreeIGL.cpp
    #SLA/Rotfinder.hpp
    #SLA/Rotfinder.cpp
    SLA/BoostAdapter.hpp
    SLA/SpatIndex.hpp
    SLA/SpatIndex.cpp
    #SLA/RasterBase.hpp
    #SLA/RasterBase.cpp
    #SLA/AGGRaster.hpp
    #SLA/RasterToPolygons.hpp
    #SLA/RasterToPolygons.cpp
    SLA/ConcaveHull.hpp
    SLA/ConcaveHull.cpp
    #SLA/Hollowing.hpp
    #SLA/Hollowing.cpp
    SLA/JobController.hpp
    SLA/SupportPoint.hpp
	SLA/SupportPointData.hpp
	SLA/SupportPointData.cpp
	SLA/SupportPointLayer.hpp
	SLA/SupportPointLayer.cpp
    SLA/SupportPointGenerator.hpp
    SLA/SupportPointGenerator.cpp
    SLA/Contour3D.hpp
    SLA/Contour3D.cpp
    SLA/IndexedMesh.hpp
    SLA/IndexedMesh.cpp
    SLA/Clustering.hpp
    SLA/Clustering.cpp
    #SLA/ReprojectPointsOnMesh.hpp
    #sla_print_test/sla_print_tests.cpp
    #sla_print_test/sla_test_utils.hpp 
	#sla_print_test/sla_test_utils.cpp
    #sla_print_test/sla_supptgen_tests.cpp
    #sla_print_test/test_utils.hpp
    #sla_print_test/catch_main.hpp
    SLA/SupportTreeData.h
    SLA/SupportTreeData.cpp
	SLA/SupportTreeHeader.h
    SLA/SupportTreeHeader.cpp
	SLA/SupportTreeNode.h
	SLA/SupportTreeNode.cpp
	
	admesh/stl.h
	admesh/connect.cpp
	admesh/normals.cpp
	admesh/shared.cpp
	admesh/stl_io.cpp
	admesh/stlinit.cpp
	admesh/util.cpp
	
	polypartition/polypartition.h
	polypartition/polypartition.cpp
	
	glu_libtess/dict.c
	glu_libtess/geom.c
	glu_libtess/memalloc.c
	glu_libtess/mesh.c
	glu_libtess/normal.c
	glu_libtess/priorityq.c
	glu_libtess/priorityq-heap.c
	glu_libtess/render.c
	glu_libtess/sweep.c
	glu_libtess/tess.c
	glu_libtess/tessmono.c
	
	poly2tri/common/shapes.cc
	poly2tri/sweep/advancing_front.cc
	poly2tri/sweep/cdt.cc
	poly2tri/sweep/sweep.cc
	poly2tri/sweep/sweep_context.cc
)

list(APPEND EXTRA_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/polypartition/
						   ${CMAKE_CURRENT_SOURCE_DIR}/glu_libtess/)
set(LIBS 
	${EXTRA_LIBS}
)

if(WIN32)
    list(APPEND LIBS Psapi.lib)
endif()

if(SLIC3R_PROFILE)
    list(APPEND LIBS Shiny)
endif()

set(EXPORT_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/../
					${CMAKE_CURRENT_SOURCE_DIR})
list(APPEND LIBS tbb)
if(TARGET tbb)
	set(DEFS SLICE3R_USE_TBB)
endif()

__add_real_target(libslic3r lib SOURCE ${SRCS}
								LIB ${LIBS}
								INC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${EXTRA_INCLUDES} ${EXPORT_INCLUDE}
								DEF ${DEFS} TBB_USE_CAPTURED_EXCEPTION=0 _CRT_SECURE_NO_WARNINGS _USE_MATH_DEFINES NOMINMAX ${EXTRA_DEFS}
								INTERFACE ${EXPORT_INCLUDE}
								SOURCE_FOLDER
								OPENMP
								)

