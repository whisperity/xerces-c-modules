/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2001-2002 The Apache Software Foundation.  All rights
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xerces" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written
 *    permission, please contact apache\@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation, and was
 * originally based on software copyright (c) 2001, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/*
 * $Id$
 */

// This class doesn't support having any children, and implements the behavior
// of an empty NodeList as far getChildNodes is concerned.
// The ParentNode subclass overrides this behavior.


#include "DOMDocumentTypeImpl.hpp"
#include "DOMElementImpl.hpp"
#include "DOMAttrImpl.hpp"
#include "DOMCasts.hpp"
#include "DOMDocumentImpl.hpp"

#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMException.hpp>

#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/XMLRegisterCleanup.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <stdio.h>
#include <assert.h>

const unsigned short DOMNodeImpl::READONLY     = 0x1<<0;
const unsigned short DOMNodeImpl::SYNCDATA     = 0x1<<1;
const unsigned short DOMNodeImpl::SYNCCHILDREN = 0x1<<2;
const unsigned short DOMNodeImpl::OWNED        = 0x1<<3;
const unsigned short DOMNodeImpl::FIRSTCHILD   = 0x1<<4;
const unsigned short DOMNodeImpl::SPECIFIED    = 0x1<<5;
const unsigned short DOMNodeImpl::IGNORABLEWS  = 0x1<<6;
const unsigned short DOMNodeImpl::SETVALUE     = 0x1<<7;
const unsigned short DOMNodeImpl::ID_ATTR      = 0x1<<8;
const unsigned short DOMNodeImpl::USERDATA     = 0x1<<9;
const unsigned short DOMNodeImpl::LEAFNODETYPE = 0x1<<10;
const unsigned short DOMNodeImpl::CHILDNODE    = 0x1<<11;
const unsigned short DOMNodeImpl::TOBERELEASED = 0x1<<12;

// -----------------------------------------------------------------------
//  Reset the singleton gEmptyNodeList
// -----------------------------------------------------------------------
static DOMNodeListImpl *gEmptyNodeList;  // make a singleton empty node list
static void reinitEmptyNodeList()
{
    delete gEmptyNodeList;
    gEmptyNodeList = 0;
}

// -----------------------------------------------------------------------
//  DOMNodeImpl Functions
// -----------------------------------------------------------------------
DOMNodeImpl::DOMNodeImpl(DOMNode *ownerNode)
{
    this->flags = 0;
    // as long as we do not have any owner, fOwnerNode is our ownerDocument
    fOwnerNode  = ownerNode;
};

// This only makes a shallow copy, cloneChildren must also be called for a
// deep clone
DOMNodeImpl::DOMNodeImpl(const DOMNodeImpl &other)
{
    this->flags = other.flags;
    this->isReadOnly(false);

    // Need to break the association w/ original parent
    this->fOwnerNode = other.getOwnerDocument();
    this->isOwned(false);
};



DOMNodeImpl::~DOMNodeImpl() {
};


DOMNode * DOMNodeImpl::appendChild(DOMNode *newChild)
{
    // Only node types that don't allow children will use this default function.
    //   Others will go to DOMParentNode::appendChild.
    throw DOMException(DOMException::HIERARCHY_REQUEST_ERR,0);
    return 0;
    //  return insertBefore(newChild, 0);
};


DOMNamedNodeMap * DOMNodeImpl::getAttributes() const {
    return 0;                   // overridden in ElementImpl
};


DOMNodeList *DOMNodeImpl::getChildNodes() const {
    static XMLRegisterCleanup emptyNodeListCleanup;

    if (gEmptyNodeList == 0)
    {
        DOMNodeList *t = new DOMNodeListImpl(0);
        if (XMLPlatformUtils::compareAndSwap((void **)&gEmptyNodeList, t, 0) != 0)
        {
            delete t;
        }
        else
        {
            emptyNodeListCleanup.registerCleanup(reinitEmptyNodeList);
        }

    }
    return (DOMNodeList *)gEmptyNodeList;
};



DOMNode * DOMNodeImpl::getFirstChild() const {
    return 0;                   // overridden in ParentNode
};


DOMNode * DOMNodeImpl::getLastChild() const
{
    return 0;                   // overridden in ParentNode
};


DOMNode * DOMNodeImpl::getNextSibling() const {
    return 0;                // overridden in ChildNode
};


const XMLCh * DOMNodeImpl::getNodeValue() const {
    return 0;                    // Overridden by anything that has a value
}


//
//  Unlike the external getOwnerDocument, this one returns the owner document
//     for document nodes as well as all of the other node types.
//
DOMDocument *DOMNodeImpl::getOwnerDocument() const
{
    if (!this->isLeafNode())
    {
        DOMElementImpl *ep = (DOMElementImpl *)castToNode(this);
        return ep->fParent.fOwnerDocument;
    }

    //  Leaf node types - those that cannot have children, like Text.
    if (isOwned()) {

        DOMDocument* ownerDoc = fOwnerNode->getOwnerDocument();

        if (!ownerDoc) {

            assert (fOwnerNode->getNodeType() == DOMNode::DOCUMENT_NODE);
            return  (DOMDocument *)fOwnerNode;
        }
        else {
            return ownerDoc;
        }
    } else {
        assert (fOwnerNode->getNodeType() == DOMNode::DOCUMENT_NODE);
        return  (DOMDocument *)fOwnerNode;
    }
};


void DOMNodeImpl::setOwnerDocument(DOMDocument *doc) {
    // if we have an owner we rely on it to have it right
    // otherwise fOwnerNode is our ownerDocument
    if (!isOwned()) {
        // revisit.  Problem with storage for doctype nodes that were created
        //                on the system heap in advance of having a document.
        fOwnerNode = doc;
    }
}

DOMNode * DOMNodeImpl::getParentNode() const
{
    return 0;                // overridden in ChildNode
};


DOMNode*  DOMNodeImpl::getPreviousSibling() const
{
    return 0;                // overridden in ChildNode
};

bool DOMNodeImpl::hasChildNodes() const
{
    return false;
};



DOMNode *DOMNodeImpl::insertBefore(DOMNode *newChild, DOMNode *refChild) {
    throw DOMException(DOMException::HIERARCHY_REQUEST_ERR, 0);
    return 0;
};


DOMNode *DOMNodeImpl::removeChild(DOMNode *oldChild)
{
    throw DOMException(DOMException::NOT_FOUND_ERR, 0);
    return 0;
};


DOMNode *DOMNodeImpl::replaceChild(DOMNode *newChild, DOMNode *oldChild)
{
    throw DOMException(DOMException::HIERARCHY_REQUEST_ERR,0);
    return 0;
};



void DOMNodeImpl::setNodeValue(const XMLCh *val)
{
    // Default behavior is to do nothing, overridden in some subclasses
};



void DOMNodeImpl::setReadOnly(bool readOnl, bool deep)
{
    this->isReadOnly(readOnl);

    if (deep) {
        for (DOMNode *mykid = castToNode(this)->getFirstChild();
            mykid != 0;
            mykid = mykid->getNextSibling()) {

            short kidNodeType = mykid->getNodeType();

            switch (kidNodeType) {
            case DOMNode::ENTITY_REFERENCE_NODE:
                break;
            case DOMNode::ELEMENT_NODE:
                ((DOMElementImpl*) mykid)->setReadOnly(readOnl, true);
                break;
            case DOMNode::DOCUMENT_TYPE_NODE:
               ((DOMDocumentTypeImpl*) mykid)->setReadOnly(readOnl, true);
               break;
            default:
                castToNodeImpl(mykid)->setReadOnly(readOnl, true);
                break;
            }
        }
    }
}


//Introduced in DOM Level 2

void DOMNodeImpl::normalize()
{
    // does nothing by default, overridden by subclasses
};


bool DOMNodeImpl::isSupported(const XMLCh *feature, const XMLCh *version) const
{
    return DOMImplementation::getImplementation()->hasFeature(feature, version);
}

const XMLCh *DOMNodeImpl::getNamespaceURI() const
{
    return 0;
}

const XMLCh *DOMNodeImpl::getPrefix() const
{
    return 0;
}

const XMLCh *DOMNodeImpl::getLocalName() const
{
    return 0;
}


void DOMNodeImpl::setPrefix(const XMLCh *fPrefix)
{
    throw DOMException(DOMException::NAMESPACE_ERR, 0);
}


bool DOMNodeImpl::hasAttributes() const {
    return 0;                   // overridden in ElementImpl
};





static const XMLCh s_xml[] = {chLatin_x, chLatin_m, chLatin_l, chNull};
static const XMLCh s_xmlURI[] =    // "http://www.w3.org/XML/1998/namespace"
    { chLatin_h, chLatin_t, chLatin_t, chLatin_p, chColon, chForwardSlash, chForwardSlash,
      chLatin_w, chLatin_w, chLatin_w, chPeriod, chLatin_w, chDigit_3, chPeriod,
      chLatin_o, chLatin_r, chLatin_g, chForwardSlash, chLatin_X, chLatin_M, chLatin_L, chForwardSlash,
      chDigit_1, chDigit_9, chDigit_9, chDigit_8, chForwardSlash,
      chLatin_n, chLatin_a, chLatin_m, chLatin_e, chLatin_s, chLatin_p, chLatin_a, chLatin_c, chLatin_e,
      chNull};
static const XMLCh s_xmlns[] = {chLatin_x, chLatin_m, chLatin_l, chLatin_n, chLatin_s, chNull};
static const XMLCh s_xmlnsURI[] = // "http://www.w3.org/2000/xmlns/"
    {  chLatin_h, chLatin_t, chLatin_t, chLatin_p, chColon, chForwardSlash, chForwardSlash,
       chLatin_w, chLatin_w, chLatin_w, chPeriod, chLatin_w, chDigit_3, chPeriod,
       chLatin_o, chLatin_r, chLatin_g, chForwardSlash,
       chDigit_2, chDigit_0, chDigit_0, chDigit_0, chForwardSlash,
       chLatin_x, chLatin_m, chLatin_l, chLatin_n, chLatin_s, chForwardSlash, chNull};


const XMLCh *DOMNodeImpl::getXmlString()      {return s_xml;};
const XMLCh *DOMNodeImpl::getXmlURIString()   {return s_xmlURI;};
const XMLCh *DOMNodeImpl::getXmlnsString()    {return s_xmlns;};
const XMLCh *DOMNodeImpl::getXmlnsURIString() {return s_xmlnsURI;};

//Return a URI mapped from the given prefix and namespaceURI as below
//    prefix   namespaceURI    output
//---------------------------------------------------
//    "xml"     xmlURI          xmlURI
//    "xml"     otherwise       NAMESPACE_ERR
//    "xmlns"   xmlnsURI        xmlnsURI (nType = ATTRIBUTE_NODE only)
//    "xmlns"   otherwise       NAMESPACE_ERR (nType = ATTRIBUTE_NODE only)
//    != null   null or ""      NAMESPACE_ERR
//    else      any             namesapceURI
const XMLCh* DOMNodeImpl::mapPrefix(const XMLCh *prefix,
                                     const XMLCh *namespaceURI, short nType)
{

    static const XMLCh s_xml[] = {chLatin_x, chLatin_m, chLatin_l, chNull};
    static const XMLCh s_xmlURI[] =    // "http://www.w3.org/XML/1998/namespace"
    { chLatin_h, chLatin_t, chLatin_t, chLatin_p, chColon, chForwardSlash, chForwardSlash,
      chLatin_w, chLatin_w, chLatin_w, chPeriod, chLatin_w, chDigit_3, chPeriod,
      chLatin_o, chLatin_r, chLatin_g, chForwardSlash, chLatin_X, chLatin_M, chLatin_L, chForwardSlash,
      chDigit_1, chDigit_9, chDigit_9, chDigit_8, chForwardSlash,
      chLatin_n, chLatin_a, chLatin_m, chLatin_e, chLatin_s, chLatin_p, chLatin_a, chLatin_c, chLatin_e,
      chNull};
    static const XMLCh s_xmlns[] = {chLatin_x, chLatin_m, chLatin_l, chLatin_n, chLatin_s, chNull};
    static const XMLCh s_xmlnsURI[] = // "http://www.w3.org/2000/xmlns/"
    {  chLatin_h, chLatin_t, chLatin_t, chLatin_p, chColon, chForwardSlash, chForwardSlash,
       chLatin_w, chLatin_w, chLatin_w, chPeriod, chLatin_w, chDigit_3, chPeriod,
       chLatin_o, chLatin_r, chLatin_g, chForwardSlash,
       chDigit_2, chDigit_0, chDigit_0, chDigit_0, chForwardSlash,
       chLatin_x, chLatin_m, chLatin_l, chLatin_n, chLatin_s, chForwardSlash, chNull};


    if (prefix == 0)
        return namespaceURI;

    if (XMLString::compareString(prefix, s_xml) == 0)  {
        if (XMLString::compareString(namespaceURI, s_xmlURI) == 0)
            return s_xmlURI;
        throw DOMException(DOMException::NAMESPACE_ERR, 0);
    } else if (nType == DOMNode::ATTRIBUTE_NODE && XMLString::compareString(prefix, s_xmlns) == 0) {
        if (XMLString::compareString(namespaceURI, s_xmlnsURI) == 0)
            return s_xmlnsURI;
        throw DOMException(DOMException::NAMESPACE_ERR, 0);
    } else if (namespaceURI == 0 || *namespaceURI == 0) {
        throw DOMException(DOMException::NAMESPACE_ERR, 0);
    } else
        return namespaceURI;
    return namespaceURI;
}

//Introduced in DOM Level 3
void* DOMNodeImpl::setUserData(const XMLCh* key, void* data, DOMUserDataHandler* handler)
{
   if (!data && !hasUserData())
       return 0;

    hasUserData(true);
    return ((DOMDocumentImpl*)getOwnerDocument())->setUserData(this, key, data, handler);
}

void* DOMNodeImpl::getUserData(const XMLCh* key) const
{
   if (hasUserData())
       return ((DOMDocumentImpl*)getOwnerDocument())->getUserData(this, key);
    return 0;
}

void DOMNodeImpl::callUserDataHandlers(DOMUserDataHandler::DOMOperationType operation,
                                       const DOMNode* src,
                                       const DOMNode* dst) const
{
    ((DOMDocumentImpl*)getOwnerDocument())->callUserDataHandlers(this, operation, src, dst);
}

bool DOMNodeImpl::isSameNode(const DOMNode* other)
{
    return (castToNode(this) == other);
}

bool DOMNodeImpl::isEqualNode(const DOMNode* arg)
{
    if (!arg)
        return false;

    if (isSameNode(arg)) {
        return true;
    }

    DOMNode* thisNode = castToNode(this);

    if (arg->getNodeType() != thisNode->getNodeType()) {
        return false;
    }

    // the compareString will check null string as well
    if (XMLString::compareString(thisNode->getNodeName(), arg->getNodeName())) {
        return false;
    }

    if (XMLString::compareString(thisNode->getLocalName(),arg->getLocalName())) {
        return false;
    }

    if (XMLString::compareString(thisNode->getNamespaceURI(), arg->getNamespaceURI())) {
        return false;
    }

    if (XMLString::compareString(thisNode->getPrefix(), arg->getPrefix())) {
        return false;
    }

    if (XMLString::compareString(thisNode->getNodeValue(), arg->getNodeValue())) {
        return false;
    }

// baseURI not suppported yet
/*
    if (XMLString::compareString(thisNode->getBaseURI(), arg->getBaseURI())) {
        return false;
    }*/

    return true;
}

const XMLCh*     DOMNodeImpl::getBaseURI() const{
    return 0;
}

short            DOMNodeImpl::compareTreePosition(DOMNode* other){
    // Questions of clarification for this method - to be answered by the
    // DOM WG.   Current assumptions listed - LM
    //
    // 1. How do ENTITY nodes compare?
    //    Current assumption: TREE_POSITION_DISCONNECTED, as ENTITY nodes
    //    aren't really 'in the tree'
    //
    // 2. How do NOTATION nodes compare?
    //    Current assumption: TREE_POSITION_DISCONNECTED, as NOTATION nodes
    //    aren't really 'in the tree'
    //
    // 3. Are TREE_POSITION_ANCESTOR and TREE_POSITION_DESCENDANT
    //    only relevant for nodes that are "part of the document tree"?
    //     <outer>
    //         <inner  myattr="true"/>
    //     </outer>
    //    Is the element node "outer" considered an ancestor of "myattr"?
    //    Current assumption: No.
    //
    // 4. How do children of ATTRIBUTE nodes compare (with eachother, or
    //    with children of other attribute nodes with the same element)
    //    Current assumption: Children of ATTRIBUTE nodes are treated as if
    //    they are the attribute node itself, unless the 2 nodes
    //    are both children of the same attribute.
    //
    // 5. How does an ENTITY_REFERENCE node compare with it's children?
    //    Given the DOM, it should precede its children as an ancestor.
    //    Given "document order",  does it represent the same position?
    //    Current assumption: An ENTITY_REFERENCE node is an ancestor of its
    //    children.
    //
    // 6. How do children of a DocumentFragment compare?
    //    Current assumption: If both nodes are part of the same document
    //    fragment, there are compared as if they were part of a document.



    DOMNode* thisNode = castToNode(this);

    // If the nodes are the same...
    if (thisNode == other)
        return (DOMNode::TREE_POSITION_SAME_NODE | DOMNode::TREE_POSITION_EQUIVALENT);

    // If either node is of type ENTITY or NOTATION, compare as disconnected
    short thisType = thisNode->getNodeType();
    short otherType = other->getNodeType();

    // If either node is of type ENTITY or NOTATION, compare as disconnected
    if (thisType == DOMNode::ENTITY_NODE ||
            thisType == DOMNode::NOTATION_NODE ||
            otherType == DOMNode::ENTITY_NODE ||
            otherType == DOMNode::NOTATION_NODE ) {
        return DOMNode::TREE_POSITION_DISCONNECTED;
    }

    //if this is a custom node, we don't really know what to do, just return
    //user should provide its own compareTreePosition logic, and shouldn't reach here
    if(thisType > 12) {
        return 0;
    }

    //if it is a custom node we must ask it for the order
    if(otherType > 12) {
        return reverseTreeOrderBitPattern(other->compareTreePosition(castToNode(this)));
    }

    // Find the ancestor of each node, and the distance each node is from
    // its ancestor.
    // During this traversal, look for ancestor/descendent relationships
    // between the 2 nodes in question.
    // We do this now, so that we get this info correct for attribute nodes
    // and their children.

    DOMNode *node;
    DOMNode *thisAncestor = castToNode(this);
    DOMNode *otherAncestor = other;
    int thisDepth=0;
    int otherDepth=0;
    for (node = castToNode(this); node != 0; node = node->getParentNode()) {
        thisDepth +=1;
        if (node == other)
            // The other node is an ancestor of this one.
            return (DOMNode::TREE_POSITION_ANCESTOR | DOMNode::TREE_POSITION_PRECEDING);
        thisAncestor = node;
    }

    for (node=other; node != 0; node = node->getParentNode()) {
        otherDepth +=1;
        if (node == castToNode(this))
            // The other node is a descendent of the reference node.
            return (DOMNode::TREE_POSITION_DESCENDANT | DOMNode::TREE_POSITION_FOLLOWING);
        otherAncestor = node;
    }


    DOMNode *otherNode = other;

    short thisAncestorType = thisAncestor->getNodeType();
    short otherAncestorType = otherAncestor->getNodeType();

    // if the ancestor is an attribute, get owning element.
    // we are now interested in the owner to determine position.

    if (thisAncestorType == DOMNode::ATTRIBUTE_NODE)  {
        thisNode = ((DOMAttrImpl *)thisAncestor)->getOwnerElement();
    }
    if (otherAncestorType == DOMNode::ATTRIBUTE_NODE) {
        otherNode = ((DOMAttrImpl *)otherAncestor)->getOwnerElement();
    }

    // Before proceeding, we should check if both ancestor nodes turned
    // out to be attributes for the same element
    if (thisAncestorType == DOMNode::ATTRIBUTE_NODE &&
            otherAncestorType == DOMNode::ATTRIBUTE_NODE &&
            thisNode==otherNode)
        return DOMNode::TREE_POSITION_EQUIVALENT;

    // Now, find the ancestor of the owning element, if the original
    // ancestor was an attribute

    if (thisAncestorType == DOMNode::ATTRIBUTE_NODE) {
        thisDepth=0;
        for (node=thisNode; node != 0; node = node->getParentNode()) {
            thisDepth +=1;
            if (node == otherNode)
                // The other node is an ancestor of the owning element
                return DOMNode::TREE_POSITION_PRECEDING;
            thisAncestor = node;
        }
        for (node=otherNode; node != 0; node = node->getParentNode()) {
            if (node == thisNode)
                // The other node is an ancestor of the owning element
                return DOMNode::TREE_POSITION_FOLLOWING;
        }
    }

    // Now, find the ancestor of the owning element, if the original
    // ancestor was an attribute
    if (otherAncestorType == DOMNode::ATTRIBUTE_NODE) {
        otherDepth=0;
        for (node=otherNode; node != 0; node = node->getParentNode()) {
            otherDepth +=1;
            if (node == thisNode)
                // The other node is a descendent of the reference
                // node's element
                return DOMNode::TREE_POSITION_FOLLOWING;
            otherAncestor = node;
        }
        for (node=thisNode; node != 0; node = node->getParentNode()) {
            if (node == otherNode)
                // The other node is an ancestor of the owning element
                return DOMNode::TREE_POSITION_PRECEDING;
        }
    }

    // thisAncestor and otherAncestor must be the same at this point,
    // otherwise, we are not in the same tree or document fragment
    if (thisAncestor != otherAncestor)
        return DOMNode::TREE_POSITION_DISCONNECTED;

    // Determine which node is of the greatest depth.
    if (thisDepth > otherDepth) {
        for (int i= 0 ; i < thisDepth - otherDepth; i++)
            thisNode = thisNode->getParentNode();
    }
    else {
        for (int i = 0; i < otherDepth - thisDepth; i++)
            otherNode = otherNode->getParentNode();
    }

    // We now have nodes at the same depth in the tree.  Find a common
    // ancestor.
    DOMNode *thisNodeP, *otherNodeP;
    for (thisNodeP = thisNode->getParentNode(),
                 otherNodeP = otherNode->getParentNode();
             thisNodeP != otherNodeP;) {
        thisNode = thisNodeP;
        otherNode = otherNodeP;
        thisNodeP = thisNodeP->getParentNode();
        otherNodeP = otherNodeP->getParentNode();
    }

    // See whether thisNode or otherNode is the leftmost
    for (DOMNode *current = thisNodeP->getFirstChild();
             current != 0;
             current = current->getNextSibling()) {
        if (current == otherNode) {
            return DOMNode::TREE_POSITION_PRECEDING;
        }
        else if (current == thisNode) {
            return DOMNode::TREE_POSITION_FOLLOWING;
        }
    }
    // REVISIT:  shouldn't get here.   Should probably throw an
    // exception
    return 0;

}

short DOMNodeImpl::reverseTreeOrderBitPattern(short pattern) const {

    if(pattern & DOMNode::TREE_POSITION_PRECEDING) {
        pattern &= !DOMNode::TREE_POSITION_PRECEDING;
        pattern |= DOMNode::TREE_POSITION_FOLLOWING;
    }
    else if(pattern & DOMNode::TREE_POSITION_FOLLOWING) {
        pattern &= !DOMNode::TREE_POSITION_FOLLOWING;
        pattern |= DOMNode::TREE_POSITION_PRECEDING;
    }

    if(pattern & DOMNode::TREE_POSITION_ANCESTOR) {
        pattern &= !DOMNode::TREE_POSITION_ANCESTOR;
        pattern |= DOMNode::TREE_POSITION_DESCENDANT;
    }
    else if(pattern & DOMNode::TREE_POSITION_DESCENDANT) {
        pattern &= !DOMNode::TREE_POSITION_DESCENDANT;
        pattern |= DOMNode::TREE_POSITION_ANCESTOR;
    }

    return pattern;
}


const XMLCh*     DOMNodeImpl::getTextContent() const{
    return 0;
}

void             DOMNodeImpl::setTextContent(const XMLCh* textContent){
    throw DOMException(DOMException::NOT_SUPPORTED_ERR, 0);
}

const XMLCh*     DOMNodeImpl::lookupNamespacePrefix(const XMLCh* namespaceURI, bool useDefault) {
    throw DOMException(DOMException::NOT_SUPPORTED_ERR, 0);
    return 0;
}

bool             DOMNodeImpl::isDefaultNamespace(const XMLCh* namespaceURI) {
    throw DOMException(DOMException::NOT_SUPPORTED_ERR, 0);
    return false;
}

const XMLCh*     DOMNodeImpl::lookupNamespaceURI(const XMLCh* prefix) {
    throw DOMException(DOMException::NOT_SUPPORTED_ERR, 0);
    return 0;
}

DOMNode*         DOMNodeImpl::getInterface(const XMLCh* feature)      {
    throw DOMException(DOMException::NOT_SUPPORTED_ERR, 0);
    return 0;
}


// non-standard extension
void DOMNodeImpl::release()
{
    // shouldn't reach here
    throw DOMException(DOMException::INVALID_ACCESS_ERR,0);
}

