#include <SBW/SignatureType.h>

std::vector<SignatureElement> *sbwSignatureType::getContents() { throw new SBWTypeMismatchException("this type is not a list");}

sbwSignatureType *sbwSignatureType::getArrayInnerType() { throw new SBWTypeMismatchException("this type is not an array"); }

Integer sbwSignatureType::getArrayDimensions() { throw new SBWTypeMismatchException("this type is not an array"); }

bool sbwSignatureType::isList() { return false; }

bool sbwSignatureType::match(sbwSignatureType *other) { return getType() == other->getType(); }

SignatureType::SignatureType() {}

SignatureType::SignatureType(sbwSignatureType *x) : SBWHandle<sbwSignatureType>(x) {}

DataBlockType SignatureType::getType() { return p->getType() ; }

std::vector<SignatureElement> *SignatureType::getListContents() { return p->getContents(); }

SignatureType SignatureType::getArrayInnerType() { return p->getArrayInnerType(); }

sbwSignatureType *SignatureType::getImpl() { return p ; }

Integer SignatureType::getArrayDimensions() { return p->getArrayDimensions(); }

bool SignatureType::match(SignatureType x) { return p->match(x.p); }
