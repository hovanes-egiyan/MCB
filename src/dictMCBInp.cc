// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIhomedIhovanesdIGlueXdIMCBdIsrcdIdictMCBInp

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
#include "/home/hovanes/GlueX/MCB/src/TUserMCBInp.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_TUserMCBInp(void *p = 0);
   static void *newArray_TUserMCBInp(Long_t size, void *p);
   static void delete_TUserMCBInp(void *p);
   static void deleteArray_TUserMCBInp(void *p);
   static void destruct_TUserMCBInp(void *p);
   static void streamer_TUserMCBInp(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TUserMCBInp*)
   {
      ::TUserMCBInp *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TUserMCBInp >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TUserMCBInp", ::TUserMCBInp::Class_Version(), "src/TUserMCBInp.h", 6,
                  typeid(::TUserMCBInp), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TUserMCBInp::Dictionary, isa_proxy, 16,
                  sizeof(::TUserMCBInp) );
      instance.SetNew(&new_TUserMCBInp);
      instance.SetNewArray(&newArray_TUserMCBInp);
      instance.SetDelete(&delete_TUserMCBInp);
      instance.SetDeleteArray(&deleteArray_TUserMCBInp);
      instance.SetDestructor(&destruct_TUserMCBInp);
      instance.SetStreamerFunc(&streamer_TUserMCBInp);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TUserMCBInp*)
   {
      return GenerateInitInstanceLocal((::TUserMCBInp*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TUserMCBInp*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr TUserMCBInp::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TUserMCBInp::Class_Name()
{
   return "TUserMCBInp";
}

//______________________________________________________________________________
const char *TUserMCBInp::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TUserMCBInp*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TUserMCBInp::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TUserMCBInp*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TUserMCBInp::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TUserMCBInp*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TUserMCBInp::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TUserMCBInp*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void TUserMCBInp::Streamer(TBuffer &R__b)
{
   // Stream an object of class TUserMCBInp.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b.ReadStaticArray((char*)bremsprocs);
      R__b >> Zcry;
      R__b >> Zamo;
      R__b >> thickn_dia;
      R__b >> thickn_amo;
      R__b >> Theta;
      R__b >> Alpha;
      R__b >> Phi;
      R__b >> E0;
      R__b >> sigE0;
      R__b >> sigx;
      R__b >> sigy;
      R__b >> sigpx;
      R__b >> sigpy;
      R__b >> coldist;
      R__b >> collength;
      R__b >> col1;
      R__b >> col2;
      R__b >> coloff_x;
      R__b >> coloff_y;
      R__b >> KLMax;
      R__b >> temperature;
      R__b >> IntTot;
      R__b.CheckByteCount(R__s, R__c, TUserMCBInp::IsA());
   } else {
      R__c = R__b.WriteVersion(TUserMCBInp::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b.WriteArray(bremsprocs, 5);
      R__b << Zcry;
      R__b << Zamo;
      R__b << thickn_dia;
      R__b << thickn_amo;
      R__b << Theta;
      R__b << Alpha;
      R__b << Phi;
      R__b << E0;
      R__b << sigE0;
      R__b << sigx;
      R__b << sigy;
      R__b << sigpx;
      R__b << sigpy;
      R__b << coldist;
      R__b << collength;
      R__b << col1;
      R__b << col2;
      R__b << coloff_x;
      R__b << coloff_y;
      R__b << KLMax;
      R__b << temperature;
      R__b << IntTot;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TUserMCBInp(void *p) {
      return  p ? new(p) ::TUserMCBInp : new ::TUserMCBInp;
   }
   static void *newArray_TUserMCBInp(Long_t nElements, void *p) {
      return p ? new(p) ::TUserMCBInp[nElements] : new ::TUserMCBInp[nElements];
   }
   // Wrapper around operator delete
   static void delete_TUserMCBInp(void *p) {
      delete ((::TUserMCBInp*)p);
   }
   static void deleteArray_TUserMCBInp(void *p) {
      delete [] ((::TUserMCBInp*)p);
   }
   static void destruct_TUserMCBInp(void *p) {
      typedef ::TUserMCBInp current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_TUserMCBInp(TBuffer &buf, void *obj) {
      ((::TUserMCBInp*)obj)->::TUserMCBInp::Streamer(buf);
   }
} // end of namespace ROOT for class ::TUserMCBInp

namespace {
  void TriggerDictionaryInitialization_dictMCBInp_Impl() {
    static const char* headers[] = {
"src/TUserMCBInp.h",
0
    };
    static const char* includePaths[] = {
"/usr/local/root/6.06.04/include",
"/home/hovanes/GlueX/MCB/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "dictMCBInp dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$src/TUserMCBInp.h")))  TUserMCBInp;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "dictMCBInp dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "src/TUserMCBInp.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"TUserMCBInp", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("dictMCBInp",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_dictMCBInp_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_dictMCBInp_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_dictMCBInp() {
  TriggerDictionaryInitialization_dictMCBInp_Impl();
}
