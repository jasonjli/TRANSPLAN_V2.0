#include "cspgraphvar.h"
#include "searchstate.h"

namespace TRANSPLAN
{
    CSPGraphVar::CSPGraphVar(int varIndex, int nodesSize, SearchState* owningState)
    :CSPVariable(varIndex, owningState)
    {
        for (int i = 0; i < nodesSize; i++)
        {
            outgoingEdges.insert(std::make_pair(i, std::set<int>()));
            incomingEdges.insert(std::make_pair(i, std::set<int>()));
        }
    }

    CSPGraphVar::CSPGraphVar(const CSPGraphVar& var)
    :CSPVariable(var)
    ,outgoingEdges(var.outgoingEdges)
    ,incomingEdges(var.incomingEdges)
    ,edges(var.edges)
    {
        //std::cout << "CSPIntVar Copy constructor called" << std::endl;
    }
    
    CSPGraphVar& CSPGraphVar::operator =(const CSPGraphVar& var)
    {
        CSPVariable::operator =(var);
        this->outgoingEdges   = var.outgoingEdges;
        this->incomingEdges   = var.incomingEdges;
        this->edges           = var.edges;
        
        return *this;
    }

    void CSPGraphVar::addEdge(int from, int to, int weight)
    {
        edges.push_back(CSPGraphEdge(edges.size(), from, to, weight));
        outgoingEdges[from].insert(edges.size() - 1);
        incomingEdges[to].insert(edges.size() - 1);
        updatedPair.push(std::make_pair(from, to));
    }

    CSP_MOD_EVENT CSPGraphVar::updateEdge(int from, int to, int weight)
    {
        bool foundEdge = false;
        std::set<int>::iterator itr = this->outgoingEdges[from].begin();

        while (itr != this->outgoingEdges[from].end())
        {
            if (this->edges[*itr].toNode = to)
            {
                foundEdge = true;
                if (this->edges[*itr].updateWeight(weight))
                {
                    if( this->edges[*itr].weight + this->getEdgeWeight(to, from) > 0 )
                    {
                        return TRANSPLAN::FAILURE;
                    }
                    else
                    {
                        updatedPair.push(std::make_pair(from, to));
                    }
                }
            }
        }

        if (!foundEdge)
        {
            addEdge(from, to, weight);
        }
        
        return TRANSPLAN::SUCCESS;
    }
    
    int CSPGraphVar::getEdgeWeight(int from, int to)
    {
        int weight = -INT_MAX;
        
        std::set<int>::iterator itr = this->outgoingEdges[from].begin();

        while (itr != this->outgoingEdges[from].end())
        {
            if (this->edges[*itr].toNode = to)
            {
                weight = this->edges[*itr].weight;
            }
        }
        
        return weight;
    }
    
    void CSPGraphVar::getOutgoingEdgesToIndexSet(int node, IntPairSet& set)
    {
        set.clear();
        std::set<int>::iterator itr = this->outgoingEdges[node].begin();

        while (itr != this->outgoingEdges[node].end())
        {
            set.insert(std::make_pair<int,int>(this->edges[*itr].toNode, this->edges[*itr].weight));
        }
    }
    
    void CSPGraphVar::getIncomingEdgesFromIndexSet(int node, IntPairSet& set)
    {
        set.clear();
        std::set<int>::iterator itr = this->incomingEdges[node].begin();

        while (itr != this->incomingEdges[node].end())
        {
            set.insert(std::make_pair<int,int>(this->edges[*itr].fromNode, this->edges[*itr].weight));
        }
    }
    
    /*!
     * Clone this variable to the targetState
     * Only called from the SearchState->Clone method
     */
    CSPGraphVar* CSPGraphVar::clone()
    {
        return new CSPGraphVar(*this);
    }

}