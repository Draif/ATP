#!/usr/bin/env python3
from argparse import ArgumentParser
import heapq
import sys

class Graph:
    edges = []
    def extendEdgesList(self, size):
        self.edges = [[0 for x in range(size)] for y in range(size)]

def setCommandArguments(parser):
    parser.add_argument("--file", "-f", dest="file",
                        required=True, help="path to file with graph")

def commandArguments():
    parser = ArgumentParser()
    setCommandArguments(parser)
    options = parser.parse_args()
    return options

def fullfilGraph(graph, filename):
    with open(filename) as f:
        size = int(f.readline())
        graph.extendEdgesList(size)
        for i in range(size - 1):
            weights = f.readline().split()
            j = i + 1
            for k in range(len(weights)):
                graph.edges[i][j] = int(weights[k])
                graph.edges[j][i] = int(weights[k])
                j += 1

def updateHeap(heap, dist):
    indexes = []
    for h in heap:
        indexes.append(h[-1])
    heap = [(dist[i],i) for i in indexes]

def MSTPrim(graph):
    tree = []
    size = len(graph.edges)
    isInHeap = [True for x in range(size)]
    dist = [sys.maxsize for x in range(size)]
    parents = [0 for x in range(size)]
    dist[0] = 0
    heap = [(dist[i], i) for i in range(size)]
    heapq.heapify(heap)
    v = heapq.heappop(heap)
    isInHeap[v[-1]] = False
    while len(heap) > 0:
        for i in range(size):
            if isInHeap[i] and graph.edges[v[-1]][i] < dist[i]:
                dist[i] = graph.edges[v[-1]][i]
                parents[i] = v[-1]
        updateHeap(heap, dist)
        print(heap)
        heapq.heapify(heap)
        v = heapq.heappop(heap)
        isInHeap[v[-1]] = False
        tree.append((parents[v[-1]], v[-1]))
    return tree

def DFS(index, graph, used, path):
    used[index] = True
    path.append(index)
    for i in range(len(graph.edges)):
        if graph.edges[index][i] > 0 and not used[i]:
            DFS(i, graph, used, path)
    path.append(index)

def HamCycle(path, size):
    used = [False for x in range(size)]
    HamCycle = []
    for n in path:
        if not used[n]:
            used[n] = True
            HamCycle.append(n)
    HamCycle.append(path[0])
    return HamCycle

def CalculatePath(hamCycle, graph):
    summ = 0
    for i in range(len(hamCycle) - 1):
        summ += graph.edges[hamCycle[i]][hamCycle[i + 1]]
    return summ

def main():
    options = commandArguments()
    graph = Graph()
    fullfilGraph(graph, options.file)
    mst = MSTPrim(graph)
    print(mst)
    newGraph = Graph()
    newGraph.extendEdgesList(len(graph.edges))
    for edge in mst:
        i = edge[0]
        j = edge[1]
        newGraph.edges[i][j] = graph.edges[i][j]
        newGraph.edges[j][i] = graph.edges[j][i]
    used = [False for x in range(len(graph.edges))]
    path = []
    DFS(0, newGraph, used, path)
    hamCycle = HamCycle(path, len(graph.edges))
    print(hamCycle)
    print("Final path = %d" % CalculatePath(hamCycle, graph))

if __name__ == "__main__":
    main()
