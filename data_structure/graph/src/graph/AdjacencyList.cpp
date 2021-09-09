#include <graph/AdjacencyList.hpp>

#include <algorithm>
#include <edge/readers/EdgeAdjacencyListReader.hpp>

namespace graph
{
    AdjacencyList::AdjacencyList(SizeG node_count) : Graph(node_count)
    {
        G = new std::vector<Node>[node_count];
    }

    AdjacencyList::AdjacencyList(AdjacencyList&& source) : Graph(std::move(source.node_count))
    {
        G = source.G;
        source.G = nullptr;
    }

    AdjacencyList::~AdjacencyList()
    {
        delete[] G;
    }

    AdjacencyList& AdjacencyList::operator=(AdjacencyList&& source)
    {
        if (this != &source)
        {
            delete[] G;
            node_count = source.node_count;
            G = source.G;
            source.G = nullptr;
            source.node_count = 0;
        }

        return *this;
    }

    std::vector<Node> AdjacencyList::get_neighbors(Node u) const
    {
        return G[u];
    }

    EdgeInputIterator AdjacencyList::edge_iterator() const
    {
        EdgeReaderPtr reader = std::shared_ptr<EdgeReader>(new EdgeAdjacencyListReader<Edge>(*this, 0, 0));
        return EdgeInputIterator(reader);
    }

    void AdjacencyList::valid_nodes(Node u, Node v) const
    {
        if (u > node_count || v > node_count)
            throw invalid_node();
        if (u < 0 || v < 0)
            throw invalid_node();
    }

    void AdjacencyList::valid_node(Node u) const
    {
        if (u > node_count || u < 0)
            throw invalid_node();
    }

    std::vector<EdgePtr> AdjacencyList::get_neighbor_edges(Node u) const
    {
        valid_node(u);
        std::vector<EdgePtr> result;
        std::transform(
            G[u].begin(),
            G[u].end(),
            std::back_inserter(result),
            [u](const int& v) {
                return std::make_shared<Edge>(u, v);
            });
        return result;
    }

    void AdjacencyList::add_edge(Node u, Node v)
    {
        valid_nodes(u, v);
        if (std::find(G[u].begin(), G[u].end(), v) == G[u].end())
            throw invalid_node();
    }

    bool AdjacencyList::has_edge(Node u, Node v) const
    {
        valid_nodes(u, v);
        return std::find(G[u].begin(), G[u].end(), v) != G[u].end();
    }

    void AdjacencyList::remove_edge(Node u, Node v)
    {
        valid_nodes(u, v);
        if (!has_edge(u, v))
            throw edge_not_found();
        G[u].erase(std::find(G[u].begin(), G[u].end(), v));
    }
}