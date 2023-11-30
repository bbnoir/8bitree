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
    intPerLine = in.readInt();
    numLines = in.readInt();
    vector<int> codeLengths(SYM_NUM, 0);
    for (int i = 0; i < SYM_NUM; i++)
        codeLengths[i] = in.readInt();
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

int8_t Decoder::decodeOne(int &line_width)
{
    if (codeTree == nullptr)
        throw std::logic_error("Code tree is null");

    const InternalNode *currentNode = &codeTree->root;
    while (true)
    {
        int temp = in.readBit();
        const Node *nextNode;
        if (temp == 0)
            nextNode = currentNode->leftChild.get();
        else if (temp == 1)
            nextNode = currentNode->rightChild.get();
        else
            throw std::logic_error("End of input before code completed");

        line_width++;

        if (dynamic_cast<const Leaf *>(nextNode) != nullptr)
            return dynamic_cast<const Leaf *>(nextNode)->symbol + SYM_MIN;
        else if (dynamic_cast<const InternalNode *>(nextNode))
            currentNode = dynamic_cast<const InternalNode *>(nextNode);
        else
            throw std::logic_error("Assertion error: Illegal node type");
    }
}

void Decoder::decode()
{
    out.write((char *)&intPerLine, sizeof(int));
    out.write((char *)&numLines, sizeof(int));
    int8_t tmp = 0;
    int line_width = 0;
    for (int i = 0; i < numLines; i++)
    {
        line_width = 0;
        for (int j = 0; j < intPerLine; j++)
        {
            tmp = decodeOne(line_width);
            out.write((char *)&tmp, sizeof(int8_t));
        }
        max_line_width = max(max_line_width, line_width);
    }
}
