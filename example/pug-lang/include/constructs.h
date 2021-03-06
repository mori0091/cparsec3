/* -*- coding: utf-8-unix -*- */
#pragma once

#include "my_parsec.h"

/**
 * \brief Constructs type object of a datatype.
 * \param name      name of the datatype
 * \param argTypes  types of the datatype's parameters
 * \return type object that represents the datatype
 */
Type makeSimpleType(Id name, Array(Type) argTypes);

/**
 * \brief Constructs type object of a funcion.
 * \param argTypes  types of the funcion's parameters.
 * \param retType   type of the function's output.
 * \return type object that represents type of the function.
 */
Type makeTypeSig(Array(Type) argTypes, Type retType);

/**
 * \brief Constructs list of unique variables.
 * \param n    number of variables
 * \return list of terms that represents list of unique variables.
 */
List(Expr) makeVars(size_t n);

/**
 * \brief Constructs a lambda abstraction.
 * \param vars    list of terms that represents list of unique variables.
 * \param body    expression of the body of lambda abstraction.
 * \return term that represents the lambda abstraction.
 */
Expr makeLambda(List(Expr) vars, Expr body);

/**
 * \brief Constructs definition of a constructor.
 * \param name      name of the constructor.
 * \param argTypes  types of the constructor's parameters
 * \param retType   type of the constructor's output.
 * \return term that represents the definition of the constructor.
 */
Expr defineConstructor(Id name, Array(Type) argTypes, Type retType);
