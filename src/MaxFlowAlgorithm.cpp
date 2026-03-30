//
// Created by guilhermecunha on 30/03/26.
//
#include "MaxFlowAlgorithm.h"

MaxFlowAlgorithm::MaxFlowAlgorithm(Graph<nodeInfo> *flowNetwork) {
    this->flowNetwork = flowNetwork;
    this->source = flowNetwork->findVertex({SOURCE, 0});
    this->sink = flowNetwork->findVertex({SINK, -1});
}
