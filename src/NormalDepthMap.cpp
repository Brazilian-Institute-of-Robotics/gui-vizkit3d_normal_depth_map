/*
 * NormalDepthMap.cpp
 *
 *  Created on: Mar 27, 2015
 *      Author: tiagotrocoli
 */

#include "NormalDepthMap.hpp"

#include <osg/Node>
#include <osg/Program>
#include <osg/ref_ptr>
#include <osg/Group>
#include <osg/Shader>
#include <osg/StateSet>
#include <osg/Uniform>
#include <osgDB/FileUtils>

#include <Definitions.hpp>

namespace normal_depth_map {

#define SHADER_FRAG "shaders/normalDepthMap.frag"
#define SHADER_VERT "shaders/normalDepthMap.vert"

NormalDepthMap::NormalDepthMap(float maxRange,
                               float maxHorizontalAngle,
                               float maxVerticalAngle) {
    _normalDepthMapNode = createTheNormalDepthMapShaderNode(
                                    maxRange,
                                    maxHorizontalAngle,
                                    maxVerticalAngle);
}

NormalDepthMap::NormalDepthMap(float maxRange,
                               float maxHorizontalAngle,
                               float maxVerticalAngle,
                               float attenuationCoeff) {
    _normalDepthMapNode = createTheNormalDepthMapShaderNode(
                                        maxRange,
                                        maxHorizontalAngle,
                                        maxVerticalAngle,
                                        attenuationCoeff);
}

NormalDepthMap::NormalDepthMap() {
    _normalDepthMapNode = createTheNormalDepthMapShaderNode();
}

void NormalDepthMap::setMaxRange(float maxRange) {
    _normalDepthMapNode->getOrCreateStateSet()
                       ->getUniform("farPlane")->set(maxRange);
}

float NormalDepthMap::getMaxRange() {
    float maxRange = 0;
    _normalDepthMapNode->getOrCreateStateSet()
                       ->getUniform("farPlane")->get(maxRange);
    return maxRange;
}

void NormalDepthMap::setMaxHorizontalAngle(float maxHorizontalAngle) {
    _normalDepthMapNode->getOrCreateStateSet()
                       ->getUniform("limitHorizontalAngle")
                       ->set(maxHorizontalAngle);
}

float NormalDepthMap::getMaxHorizontalAngle() {
    float maxHorizontalAngle = 0;
    _normalDepthMapNode->getOrCreateStateSet()
                       ->getUniform("limitHorizontalAngle")
                       ->get(maxHorizontalAngle);
    return maxHorizontalAngle;
}

void NormalDepthMap::setMaxVerticalAngle(float maxVerticalAngle) {
    _normalDepthMapNode->getOrCreateStateSet()
                       ->getUniform("limitVerticalAngle")
                       ->set(maxVerticalAngle);
}

float NormalDepthMap::getMaxVerticalAngle() {
    float maxVerticalAngle = 0;
    _normalDepthMapNode->getOrCreateStateSet()
                       ->getUniform("limitVerticalAngle")
                       ->get(maxVerticalAngle);
    return maxVerticalAngle;
}

void NormalDepthMap::setAttenuationCoefficient(float coefficient) {
    _normalDepthMapNode->getOrCreateStateSet()
                       ->getUniform("attenuationCoeff")->set(coefficient);
}

float NormalDepthMap::getAttenuationCoefficient() {
    float coefficient = 0;
    _normalDepthMapNode->getOrCreateStateSet()
                       ->getUniform("attenuationCoeff")->get(coefficient);
    return coefficient;
}

void NormalDepthMap::setDrawNormal(bool drawNormal) {
    _normalDepthMapNode->getOrCreateStateSet()
                       ->getUniform("drawNormal")->set(drawNormal);
}

bool NormalDepthMap::isDrawNormal() {
    bool drawNormal;
    _normalDepthMapNode->getOrCreateStateSet()
                       ->getUniform("drawNormal")->get(drawNormal);
    return drawNormal;
}

void NormalDepthMap::setDrawDepth(bool drawDepth) {
    _normalDepthMapNode->getOrCreateStateSet()
                       ->getUniform("drawDepth")->set(drawDepth);
}

bool NormalDepthMap::isDrawDepth() {
    bool drawDepth;
    _normalDepthMapNode->getOrCreateStateSet()
                       ->getUniform("drawDepth")->get(drawDepth);
    return drawDepth;
}

void NormalDepthMap::addNodeChild(osg::ref_ptr<osg::Node> node) {
    _normalDepthMapNode->addChild(node);
}

osg::ref_ptr<osg::Group> NormalDepthMap::createTheNormalDepthMapShaderNode(
                                                float maxRange,
                                                float maxHorizontalAngle,
                                                float maxVerticalAngle,
                                                float attenuationCoefficient,
                                                bool drawDepth,
                                                bool drawNormal) {

    osg::ref_ptr<osg::Group> localRoot = new osg::Group();
    osg::ref_ptr<osg::Program> program(new osg::Program());

    std::string vert_path = std::string(RESOURCE_PATH)
                          + std::string(SHADER_VERT);

    std::string frag_path = std::string(RESOURCE_PATH)
                          + std::string(SHADER_FRAG);

    osg::ref_ptr<osg::Shader> shaderVertex = osg::Shader::readShaderFile(
                                                        osg::Shader::VERTEX,
                                                        vert_path);
    osg::ref_ptr<osg::Shader> shaderFragment = osg::Shader::readShaderFile(
                                                        osg::Shader::FRAGMENT,
                                                        frag_path);
    program->addShader(shaderFragment);
    program->addShader(shaderVertex);

    osg::ref_ptr<osg::StateSet> ss = localRoot->getOrCreateStateSet();
    ss->setAttribute(program);

    ss->addUniform(new osg::Uniform("attenuationCoeff", attenuationCoefficient));
    ss->addUniform(new osg::Uniform("farPlane", maxRange));
    ss->addUniform(new osg::Uniform("limitHorizontalAngle", maxHorizontalAngle));
    ss->addUniform(new osg::Uniform("limitVerticalAngle", maxVerticalAngle));
    ss->addUniform(new osg::Uniform("drawNormal", drawNormal));
    ss->addUniform(new osg::Uniform("drawDepth", drawDepth));

    return localRoot;
}

}
