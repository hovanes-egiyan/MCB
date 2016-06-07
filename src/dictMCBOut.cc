// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedIhovanesdIGlueXdIMCBdIsrcdIdictMCBOut

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "/home/hovanes/GlueX/MCB/src/TUserMCBOut.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_TUserMCBOut(void *p = 0);
   static void *newArray_TUserMCBOut(Long_t size, void *p);
   static void delete_TUserMCBOut(void *p);
   static void deleteArray_TUserMCBOut(void *p);
   static void destruct_TUserMCBOut(void *p);
   static void streamer_TUserMCBOut(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TUserMCBOut*)
   {
      ::TUserMCBOut *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TUserMCBOut >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TUserMCBOut", ::TUserMCBOut::Class_Version(), "src/TUserMCBOut.h", 6,
                  typeid(::TUserMCBOut), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TUserMCBOut::Dictionary, isa_proxy, 16,
                  sizeof(::TUserMCBOut) );
      instance.SetNew(&new_TUserMCBOut);
      instance.SetNewArray(&newArray_TUserMCBOut);
      instance.SetDelete(&delete_TUserMCBOut);
      instance.SetDeleteArray(&deleteArray_TUserMCBOut);
      instance.SetDestructor(&destruct_TUserMCBOut);
      instance.SetStreamerFunc(&streamer_TUserMCBOut);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TUserMCBOut*)
   {
      return GenerateInitInstanceLocal((::TUserMCBOut*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TUserMCBOut*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr TUserMCBOut::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TUserMCBOut::Class_Name()
{
   return "TUserMCBOut";
}

//______________________________________________________________________________
const char *TUserMCBOut::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TUserMCBOut*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TUserMCBOut::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TUserMCBOut*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TUserMCBOut::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TUserMCBOut*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TUserMCBOut::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TUserMCBOut*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void TUserMCBOut::Streamer(TBuffer &R__b)
{
   // Stream an object of class TUserMCBOut.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b.ReadStaticArray((char*)bremsprocs);
      R__b >> MaxH;
      R__b >> MaxKL;
      R__b >> IntensityMin;
      R__b >> IntensityMax;
      R__b >> IntensityTot;
      R__b >> NormVolume;
      R__b >> no_Electrons;
      R__b >> no_Photons;
      R__b >> no_OutOfBounds;
      R__b >> no_CollPhotons;
      R__b.ReadStaticArray((int*)RandomVectors);
      R__b.CheckByteCount(R__s, R__c, TUserMCBOut::IsA());
   } else {
      R__c = R__b.WriteVersion(TUserMCBOut::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b.WriteArray(bremsprocs, 5);
      R__b << MaxH;
      R__b << MaxKL;
      R__b << IntensityMin;
      R__b << IntensityMax;
      R__b << IntensityTot;
      R__b << NormVolume;
      R__b << no_Electrons;
      R__b << no_Photons;
      R__b << no_OutOfBounds;
      R__b << no_CollPhotons;
      R__b.WriteArray(RandomVectors, 25);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TUserMCBOut(void *p) {
      return  p ? new(p) ::TUserMCBOut : new ::TUserMCBOut;
   }
   static void *newArray_TUserMCBOut(Long_t nElements, void *p) {
      return p ? new(p) ::TUserMCBOut[nElements] : new ::TUserMCBOut[nElements];
   }
   // Wrapper around operator delete
   static void delete_TUserMCBOut(void *p) {
      delete ((::TUserMCBOut*)p);
   }
   static void deleteArray_TUserMCBOut(void *p) {
      delete [] ((::TUserMCBOut*)p);
   }
   static void destruct_TUserMCBOut(void *p) {
      typedef ::TUserMCBOut current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_TUserMCBOut(TBuffer &buf, void *obj) {
      ((::TUserMCBOut*)obj)->::TUserMCBOut::Streamer(buf);
   }
} // end of namespace ROOT for class ::TUserMCBOut

namespace {
  void TriggerDictionaryInitialization_dictMCBOut_Impl() {
    static const char* headers[] = {
"src/TUserMCBOut.h",
0
    };
    static const char* includePaths[] = {
"/usr/local/root/6.06.04/include",
"/home/hovanes/GlueX/MCB/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "dictMCBOut dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$src/TUserMCBOut.h")))  TUserMCBOut;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "dictMCBOut dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "src/TUserMCBOut.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"TUserMCBOut", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("dictMCBOut",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_dictMCBOut_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_dictMCBOut_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_dictMCBOut() {
  TriggerDictionaryInitialization_dictMCBOut_Impl();
}
