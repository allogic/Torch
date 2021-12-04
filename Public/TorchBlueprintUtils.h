#pragma once

#include "CoreMinimal.h"

template<typename T>
static bool GetPrimitiveByName(UObject* target, const FName& name, typename const T::TCppType& value)
{
  if (target)
  {
    FProperty* prop = target->GetClass()->FindPropertyByName(name);
    if (prop)
    {
      typename T::TCppType* ptr = prop->ContainerPtrToValuePtr<typename T::TCppType>(target);
      if (ptr)
      {
        value = *ptr;
        return true;
      }
    }
  }
  return false;
}

template<typename T>
static bool SetPrimitiveByName(UObject* target, const FName& name, typename const T::TCppType& value)
{
  if (target)
  {
    FProperty* prop = target->GetClass()->FindPropertyByName(name);
    if (prop)
    {
      typename T::TCppType* ptr = prop->ContainerPtrToValuePtr<typename T::TCppType>(target);
      if (ptr)
      {
        *ptr = value;
        return true;
      }
    }
  }
  return false;
}

template<typename T>
static bool GetStructureByName(UObject* target, const FName& name, T& value)
{
  if (target)
  {
    FProperty* prop = target->GetClass()->FindPropertyByName(name);
    if (prop)
    {
      void* ptr = prop->ContainerPtrToValuePtr<void>(target);
      if (ptr)
      {
        if (FStructProperty* structProp = CastField<FStructProperty>(prop))
        {
          if (structProp->Struct == TBaseStructure<T>::Get())
          {
            structProp->CopyCompleteValue(&value, ptr);
            return true;
          }
        }
      }
    }
  }
  return false;
}

template<typename T>
static bool SetStructureByName(UObject* target, const FName& name, const T& value)
{
  if (target)
  {
    FProperty* prop = target->GetClass()->FindPropertyByName(name);
    if (prop)
    {
      void* ptr = prop->ContainerPtrToValuePtr<void>(target);
      if (ptr)
      {
        if (FStructProperty* structProp = CastField<FStructProperty>(prop))
        {
          if (structProp->Struct == TBaseStructure<T>::Get())
          {
            structProp->CopyCompleteValue(ptr, &value);
            return true;
          }
        }
      }
    }
  }
  return false;
}