//-----------------------------------------------
// Copyright 2009 Wellcome Trust Sanger Institute
// Written by Jared Simpson (js18@sanger.ac.uk)
// Released under the GPL license
//-----------------------------------------------
//
// String Graph - Bidirectional graph of sequence reads
// and their overlaps
// Inherits from Bigraph::Vertex/Bigraph::Edge
//
#ifndef CONTIGGRAPH_H
#define CONTIGGRAPH_H

#include "Bigraph.h"
#include <cassert>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <getopt.h>
#include <sstream>
#include <string>

typedef Bigraph StringGraph;
class StringVertex;
class StringEdge;

// String edge sorting function, by length
struct StringEdgeLenComp
{
	bool operator()(const Edge* pA, const Edge* pB);
};


// Derived from Bigraph Edge
class StringEdge : public Edge
{
	public:

		// constructors
		StringEdge(Vertex* start, 
                   Vertex* end, 
		    	   EdgeDir dir, 
			       EdgeComp comp, 
			       std::string s) : Edge(start, end, dir, comp), m_seq(s) {}
		
		// functions
		virtual void merge(const Edge* pEdge);
		virtual void flip();
		void updateLabel(const StringEdge* pSE);
		virtual std::string getLabel() const;
		const std::string& getSeq() const { return m_seq; }
		size_t getSeqLen() const { return m_seq.length(); }
		
		
	private:

		std::string m_seq;
};

// Derived from Bigraph vertex
class StringVertex : public Vertex
{
	public:
		// constructors
		StringVertex(VertexID id, const std::string& s) : Vertex(id), m_seq(s), m_readCount(1) {}
		
		// functions
		virtual void merge(const Edge* pEdge);
		virtual void validate() const;
		virtual void sortAdjList();

		size_t getReadCount() const { return m_readCount; }
		const std::string& getSeq() const { return m_seq; }

	private:
		std::string m_seq;
		size_t m_readCount;
};

// Visit functors
struct SGFastaVisitor
{
	// constructor
	SGFastaVisitor(std::string filename) : m_fileHandle(filename.c_str()) {}
	~SGFastaVisitor() { m_fileHandle.close(); }

	// functions
	void previsit(StringGraph* /*pGraph*/) {}
	bool visit(StringGraph* pGraph, Vertex* pVertex);
	void postvisit(StringGraph* /*pGraph*/) {}
	// data
	std::ofstream m_fileHandle;
};

struct SGTransRedVisitor
{
	SGTransRedVisitor() {}
	void previsit(StringGraph* pGraph);
	bool visit(StringGraph* pGraph, Vertex* pVertex);
	void postvisit(StringGraph*);
};

#endif
