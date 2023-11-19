#include "Decoder.h"
#include "CodeTree.h"
#include "Constants.h"
#include "Utils.h"
#include <stdexcept>
#include <vector>
#include <sstream>
#include <algorithm>

Decoder::Decoder(string encodedFileName, string decodedFileName) : in(encodedFileName), out(decodedFileName)
{
    vector<int> codeLengths;
    string line;
    getline(in, line);
    stringstream ss(line);
    int length;
    while (ss >> length)
        codeLengths.push_back(length);
    codeTree = CanonicalToCodeTree(codeLengths);
    max_line_width = 0;
}

CodeTree *Decoder::CanonicalToCodeTree(vector<int> codeLengths)
{
    vector<std::unique_ptr<Node>> nodes;
    for (int i = *std::max_element(codeLengths.cbegin(), codeLengths.cend());; i--)
    { // Descend through code lengths
        if (nodes.size() % 2 != 0)
            throw std::logic_error("Assertion error: Violation of canonical code invariants");
        vector<std::unique_ptr<Node>> newNodes;

        // Add leaves for symbols with positive code length i
        if (i > 0)
        {
            int j = 0;
            for (int cl : codeLengths)
            {
                if (cl == i)
                    newNodes.push_back(std::unique_ptr<Node>(new Leaf(j)));
                j++;
            }
        }

        // Merge pairs of nodes from the previous deeper layer
        for (std::size_t j = 0; j < nodes.size(); j += 2)
        {
            newNodes.push_back(std::unique_ptr<Node>(new InternalNode(
                std::move(nodes.at(j)), std::move(nodes.at(j + 1)))));
        }
        nodes = std::move(newNodes);

        if (i == 0)
            break;
    }

    if (nodes.size() != 1)
        throw std::logic_error("Assertion error: Violation of canonical code invariants");

    Node *temp = nodes.front().release();
    InternalNode *root = dynamic_cast<InternalNode *>(temp);
    CodeTree *result = new CodeTree(std::move(*root), static_cast<int>(codeLengths.size()));
    delete root;
    return result;
}

string Decoder::readline()
{
    if (codeTree == nullptr)
        throw std::logic_error("Code tree is null");

    const InternalNode *currentNode = &codeTree->root;
    string line;
    getline(in, line);
    max_line_width = std::max(max_line_width, static_cast<int>(line.length()));
    stringstream ss(line);
    char temp;
    stringstream result;
    while (ss >> temp)
    {
        const Node *nextNode;
        if (temp == '0')
            nextNode = currentNode->leftChild.get();
        else if (temp == '1')
            nextNode = currentNode->rightChild.get();
        else
            throw std::logic_error("Assertion error: Invalid value from input.get()");

        if (dynamic_cast<const Leaf *>(nextNode) != nullptr)
        {
            result << dynamic_cast<const Leaf *>(nextNode)->symbol + SYM_MIN << " ";
            currentNode = &codeTree->root;
        }
        else if (dynamic_cast<const InternalNode *>(nextNode))
            currentNode = dynamic_cast<const InternalNode *>(nextNode);
        else
            throw std::logic_error("Assertion error: Illegal node type");
    }
    return result.str();
}

bool Decoder::notEOF()
{
    return !in.eof();
}

void Decoder::decode()
{
    while (notEOF())
    {
        out << readline();
        if (notEOF())
            out << endl;
    }
}

void Decoder::decode(int numLines)
{
    int progress = 0;
    while (notEOF())
    {
        progressBar("Decode", progress++, numLines);
        out << readline();
        if (notEOF())
            out << endl;
    }
}
