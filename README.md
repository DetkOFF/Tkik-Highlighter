# Code Highlighter (Colorizer) for "test language"

## "test language" tokens:

|               **token**              |        **description**        |
|:------------------------------------:|:-----------------------------:|
| _varName                             | variable declaration          |
| [0-9]* (ex. 42)                      | integer number value          |
| '[a-zA-Z][a-zA-Z0-9 ]*' (ex. 'text') | string value                  |
| +, -, *, /                           | plus, minus, multiply, divide |
| :=                                   | assignment operator           |
| =                                    | equals                        |
| if                                   |                               |
| else                                 |                               |
| while                                |                               |
| return                               |                               |
| func                                 | function declaration keyword  |
| ( ), { }                             |                               |

## "test language"code example
```
_a := 15
_b := 'abc'
if(_b = 'abcde'){
    _a := _a*2
}
else{
    _a := _a*3
}
while(_a = 45)
    _a:= 11
return 0
```
