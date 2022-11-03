#include "glarea_setting.h"

void GLAreaSetting::initGlobalParameterList(RichParameterList& defaultGlobalParamSet)
{
	defaultGlobalParamSet.addParam(RichColor(backgroundBotColorParam(),QColor(128,128,255),"MeshLab Bottom BackGround Color","MeshLab GLarea's BackGround Color(bottom corner)"));
    defaultGlobalParamSet.addParam(RichColor(backgroundTopColorParam(),QColor(  0, 0,   0),"MeshLab Top BackGround Color","MeshLab GLarea's BackGround Color(top corner)"));
	defaultGlobalParamSet.addParam(RichColor(logAreaColorParam(),   QColor(128,16,16),"MeshLab GLarea's Log Area Color","MeshLab GLarea's BackGround Color(bottom corner)"));
	defaultGlobalParamSet.addParam(RichColor(textColorParam(),vcg::Color4b::White,"Text Color","Color of the text used in all the Graphics Window (it should be well different from the background color...)"));


    defaultGlobalParamSet.addParam(RichColor(baseLightAmbientColorParam()	,QColor( 32, 32, 32),"MeshLab Base Light Ambient Color","MeshLab GLarea's BackGround Color(bottom corner)"));
	defaultGlobalParamSet.addParam(RichColor(baseLightDiffuseColorParam()	,QColor(204,204,204),"MeshLab Base Light Diffuse Color","MeshLab GLarea's BackGround Color(top corner)"));
	defaultGlobalParamSet.addParam(RichColor(baseLightSpecularColorParam() ,QColor(255,255,255),"MeshLab Base Light Specular Color","MeshLab GLarea's BackGround Color(bottom corner)"));

	defaultGlobalParamSet.addParam(RichColor(fancyBLightDiffuseColorParam()	,QColor(255,204,204),"MeshLab Base Light Diffuse Color","MeshLab GLarea's BackGround Color(top corner)"));
	defaultGlobalParamSet.addParam(RichColor(fancyFLightDiffuseColorParam()	,QColor(204,204,255),"MeshLab Base Light Diffuse Color","MeshLab GLarea's BackGround Color(top corner)"));

	QStringList textureMinFilterModes =  (QStringList() << "Nearest" << "MipMap");
	QStringList textureMagFilterModes =  (QStringList() << "Nearest" << "Linear");
	defaultGlobalParamSet.addParam(RichEnum(textureMinFilterParam()	, 1,textureMinFilterModes,"MeshLab Texture Minification Filtering","MeshLab GLarea's BackGround Color(top corner)"));
	defaultGlobalParamSet.addParam(RichEnum(textureMagFilterParam()	, 1,textureMagFilterModes,"MeshLab Texture Magnification Filtering","MeshLab GLarea's BackGround Color(top corner)"));

	defaultGlobalParamSet.addParam(RichBool(pointDistanceAttenuationParam()	, true,"Perspective Varying Point Size","If true the size of the points is drawn with a size proprtional to the distance from the observer."));
	defaultGlobalParamSet.addParam(RichBool(pointSmoothParam()	, false,"Antialiased Point","If true the points are drawn with small circles instead of fast squared dots."));
	defaultGlobalParamSet.addParam(RichFloat(pointSizeParam()	, 2.0, "Point Size","The base size of points when drawn"));

	defaultGlobalParamSet.addParam(RichBool(wheelDirectionParam(), false, "Wheel Direction", "If true, inverts the direction of the mouse wheel for zooming in/out in the MeshLab canvas."));
	defaultGlobalParamSet.addParam(RichInt(matrixDecimalPrecisionParam(), 2, "Rotation Matrix Precision", "Number of decimal values shown in the rotation matrix"));
}


void GLAreaSetting::updateGlobalParameterSet( const RichParameterList& rps )
{
	logAreaColor = rps.getColor4b(logAreaColorParam());
	backgroundBotColor =  rps.getColor4b(backgroundBotColorParam());
	backgroundTopColor =  rps.getColor4b(backgroundTopColorParam());

	baseLightAmbientColor =  rps.getColor4b(baseLightAmbientColorParam()	);
	baseLightDiffuseColor =  rps.getColor4b(baseLightDiffuseColorParam()	);
	baseLightSpecularColor =  rps.getColor4b(baseLightSpecularColorParam() );

	fancyBLightDiffuseColor =  rps.getColor4b(fancyBLightDiffuseColorParam());
	fancyFLightDiffuseColor =  rps.getColor4b(fancyFLightDiffuseColorParam());

	textureMinFilter = rps.getEnum(this->textureMinFilterParam());
	textureMagFilter = rps.getEnum(this->textureMagFilterParam());

	pointDistanceAttenuation = rps.getBool(this->pointDistanceAttenuationParam());
	pointSmooth = rps.getBool(this->pointSmoothParam());
	pointSize = rps.getFloat(this->pointSizeParam());
	wheelDirection = rps.getBool(this->wheelDirectionParam());
	matrixDecimalPrecision = rps.getInt(this->matrixDecimalPrecisionParam());
	currentGlobalParamSet=&rps;
}
