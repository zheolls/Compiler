indentifer:
variable
func
customtype

assignmentoperator:
+=
-=
*=
/=
=
%=


base_type:
int
double
void
bool
long 
short

type:
base_type
customtype

array_state:
type variable dimension
type * variable dimension

dimension:
[ arraylen ] dimension
[ arraylen ]

arraylen:
integer
blank

constant state:
const variable state

customtype state:
struct customtype {
	start
}

start:
variable state ; start
func state ; start
constant state ; start


variable state:
type variable
type * variable
type & variable
array_state

func state:
type func (expression)
type * func (expression)
type & func (expression)
func state { block code }

func_expression:
func(expression)

e:
variable
func_expression
number
constant
false
true

expression:
expression_14
expression , expression_14

expression_14:
expression_13
expression_14 assignmentoperator expression_13

expression_13:
expression_12
expression_12 ? expression_13 : expression_13

expression_12:
expression_11
expression_12 || expression_11

expression_11:
expression_10
expression_11 && expression_10

expression_10:
expression_9
expression_10 | expression_9

expression_9:
expression_8
expression_9 ^ expression_8

expression_8:
expression_7
expression_8 & expression_7

expression_7:
expression_6
expression_7 == expression_6
expression_7 != expression_6

expression_6:
expression_5
expression_6 > expression_5
expression_6 >= expression_5
expression_6 < expression_5
expression_6 >= expression_5

expression_5:
expression_4
expression_5 << expression_4
expression_5 >> expression_4

expression_4:
expression_3
expression_4 + expression_3
expression_4 - expression_3

expression_3:
expression_2
expression_3 * expression_2
expression_3 / expression_2
expression_3 % expression_2

expression_2:
expression_1
sizeof expression_1
~ expression_1
! expression_1
& expression_1
* expression_1
expression_1 --
-- expression_1
expression_1 ++
++ expression_1
( type ) expression_1
- expression_1

expression_1:
expression1 . expression
expression1 -> expression
(expression)
expression_1 [ expression ]
e
variable state

while_expression:
while ( expression ) sentence

block code:
variable state ; block code
constant state ; block code
sentence
blank

for_condition:
expression
blank

for_expression:
for (for_condition ; for_condition ; for_condition) sentence

if_expression:
if (expression) sentence else sentence 
if (expression) sentence 

sentence:
for_expression
if_expression
while_expression
expression ;
{ block code }