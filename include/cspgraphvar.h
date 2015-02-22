/* 
 * File:   cspgraphvar.h
 * Author: debdeep
 *
 * Created on 16 November 2014, 2:25 PM
 */

#ifndef CSPGRAPHVAR_H
#define	CSPGRAPHVAR_H

#include "constants.h"
#include "cspvariable.h"

namespace TRANSPLAN
{

class CSPGraphVar : public CSPVariable
{
    class CSPGraphEdge
    {
    public:
        int index;
        int fromNode;
        int toNode;
        int weight;
        int maxWeight; ///maximum weight

        CSPGraphEdge(int index, int from, int to, int weight, int maxWeight = INT_MAX)
        : index(index)
        , fromNode(from)
        , toNode(to)
        , weight(weight)
        , maxWeight(maxWeight)
        {
        }

        CSPGraphEdge(const CSPGraphEdge& edge)
        : index(edge.index)
        , fromNode(edge.fromNode)
        , toNode(edge.toNode)
        , weight(edge.weight)
        , maxWeight(edge.maxWeight)
        {
        }
        
        CSPGraphEdge& operator=(const CSPGraphEdge& edge)
        {
            this->index    = edge.index;
            this->fromNode = edge.fromNode;
            this->toNode   = edge.toNode;
            this->weight   = edge.weight;  
            this->maxWeight = edge.maxWeight;
            return *this;
        }

        bool updateWeight(int newWeight)
        {
        	if( newWeight > maxWeight )
        	{
        		return false;
        	}
        	else if (weight <= newWeight)
            {
                weight = newWeight;
                return true;
            }
            else
            {
                return false;
            }
        }
    };
public:

    IntIntSetMap outgoingEdges;
    IntIntSetMap incomingEdges;
    std::vector<CSPGraphEdge> edges;
    std::stack< IntPair > updatedPair;

    CSPGraphVar(int varIndex, int nodesSize, SearchState* state);

    CSPGraphVar(const CSPGraphVar& var);
    
    CSPGraphVar& operator=(const CSPGraphVar& var);

    void addEdge(int from, int to, int weight, int maxWeight);

    CSP_MOD_EVENT updateEdge(int from, int to, int weight);
    
    int getEdgeWeight(int from, int to);
    
    void getOutgoingEdgesToIndexSet(int node, IntPairSet& set);
    
    void getIncomingEdgesFromIndexSet(int node, IntPairSet& set);
    
    CSPGraphVar* clone();
};
}

#endif	/* CSPGRAPHVAR_H */

