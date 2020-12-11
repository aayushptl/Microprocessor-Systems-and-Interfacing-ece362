/*
 * parsing.h
 *
 *  Created on: Nov 19, 2019
 *      Author: terrencerandall
 */

#ifndef PARSING_H_
#define PARSING_H_

//BUTTON CODES
#define INIT_VALUE      -1
#define ZERO            0
#define ONE             1
#define TWO             2
#define THREE           3
#define FOUR            4
#define FIVE            5
#define SIX             6
#define SEVEN           7
#define EIGHT           8
#define NINE            9
#define DECIMAL         10
#define NEGATIVE_SIGN   11
#define PLUS            12
#define MINUS           13
#define DIVIDE          14
#define MULTIPLY        15
#define E_TO_THE_X      16
#define NATURAL_LOG     17
#define LOG10           18
#define CARROT          19
#define OPEN_PAREN      20
#define CLOSE_PAREN     21
#define SIN             22
#define COS             23
#define TAN             24
#define ARCSIN          25
#define ARCCOS          26
#define ARCTAN          27
#define FACTORIAL       28
#define X_VARIABLE      29
#define GRAPH           30
#define DEG_TO_RAD      31
#define RAD_TO_DEG      32

//STATUS CODE
#define NORMAL          0
#define ALTERNATE_1     1
#define ALTERNATE_2     2

//Function declarations
int addToStack(int *, int adding);
int clearStack(int *);


#endif /* PARSING_H_ */
