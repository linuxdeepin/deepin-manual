# Calculator|../common/deepin-calculator.svg|

## Overview

Calculator is a simple and easy-to-use desktop calculator. It supports standard mode and scientific mode, with keyboard input perfectly matched, as well as symbolic fault-tolerant computing and calculation result linkage.

## Operations

Calculator supports standard mode and scientific mode. In standard mode, you can execute the four fundamental operations of arithmetic(addition, subtraction, multiplication and division). And in scientific mode, you can calculate high-level operations such as function, exponent, root, and so on. The standard mode is set by default, click ![icon_menu](icon/icon_menu.svg) > **Mode** to select the one you want.

### Standard Mode


​                            ![0|calculator](jpg/standard mode.png)

| Icon                                | Name                                                | Description                                                  |
| ----------------------------------- | :-------------------------------------------------- | ------------------------------------------------------------ |
| 0~9                                 | Number Key                                          | Basic Arabic numerals                                        |
| ![mc](icon/MC.svg)                  | Clear Key                                           | Clear all memories                                           |
| ![mr](icon/MR.svg)                  | Storage Key                                         | Memory recall                                                |
| ![m+](icon/M+.svg)                  | Storage Key                                         | Memory add; click ![m+](icon/M+.svg) to add the current number accumulatively to the memory and interrupt digital input. |
| ![m-](icon/M-.svg)                  | Storage Key                                         | Memory subtract; click ![m-](icon/M-.svg) to subtract the current number from the memory and interrupt digital input. |
| ![ms](icon/MS.svg)                  | Storage Key                                         | Memory store；click ![ms](icon/MS.svg) to add the numeric value in the input box to the memory list. |
| ![M](icon/M.svg)/![m^](icon/M^.svg) | Storage Key                                         | Click ![M](icon/M.svg)to expand the memory list; click ![m^](icon/M^.svg) to fold the memory list. The memory will be cleared up when Calculator is closed. |
| C                                   | Clear                                               | Click once to clear current contents and click twice to clear all. |
| %                                   | Percent Sign                                        | To input percent sign                                        |
| ![delete](icon/delete.svg)          | Delete                                              | Click once to delete a character forward                     |
| +-×÷                                | Addition, subtraction, multiplication, and division | Basic math operators for addition, subtraction, multiplication and division |
| .                                   | Decimal Point                                       | To input decimal point                                       |
| ()                                  | Bracket                                             | To input brackets with the left and right bracket completed automatically. |
| =                                   | Equal Sign                                          | To get result                                                |

### Scientific Mode

![0|scientific](jpg/scientific mode.png)



| Icon                                                         | Name       | Description                                                         |
| ------------------------------------------------------------ | :--------- | ------------------------------------------------------------ |
| F-E                                                          | E-notation   | Click F-E to startup E-notation. Click ![icon](icon/F-E.svg) to close E-notation. |
| ![icon](jpg/deg_normal.png)/![icon](jpg/grad_normal.png)/![icon](jpg/rad_normal.png) | Metric conversions   | It calculates the angle, gradient and radian respectively. |
| sin、cos、tan、cot                                           | Trigonometric functions | It calculates the sine, cosine, tangent and cotangent of the numerical value respectively. |
| &#124;x&#124;、Rand                                          | F functions | It calculates the absolute value of the value and randomly display a 31 bit number. |
| 2<sup>nd</sup>                                               | 2<sup>nd</sup> function key | Click 2<sup>nd</sup> to switch to anti trigonometric function and open root operation interface ; click ![icon](icon/2nd.svg) to switch to trigonometric functions and power operations. |
| x<sup>2</sup>、x<sup>3</sup> 、x<sup>y</sup>、10<sup>x</sup> | Power calculation | It calculates the square, cubic, y-power of numerical values and x-power of 10 respectively. |
| log、In                                                      | Logarithmic function | It calculates the logarithms based on 10 and E, respectively. |
| π                                                          | PI                                      | Approximately equals to 3.14159......, which can be accurate to 31 digits after the decimal point. |
| e                                                            | Constant | Approximately equals to 2.71828....., which can be accurate to 31 digits after the decimal point. |
| Mod                                                          | Remainder | It displays the modulus or remainder of x / y. |
| 1/x                                                          | Inverse proportional function | It calculates the reciprocal of the displayed value. |
| x!                                                           | Factorial | It calculates the factorial of the displayed number. |
| exp                                                          | Exponent | It allows you to enter numbers in scientific notation. |
| sin<sup>-1</sup>、cos<sup>-1</sup>、tan<sup>-1</sup>、cot<sup>-1</sup> | Anti-trigonometric function | It calculates the anti-trigonometric functions of sin, cos, tan and cot respectively. |
| ![icon](jpg/square root_normal.png)、![icon](jpg/cube root_normal.png)、![icon](jpg/y root_normal.png) | Rooting operations | It calculates the square root, cube root and y-th root of X respectively. |
| 2<sup>x</sup>                                                | Power operations | It calculates x-power of 2.                  |
| log<sub>y</sub><sup>x</sup>                                  | Logarithmic function | It calculates the logarithm of x with y as the base. |
| e<sup>x</sup>                                                | Exponential function | It calculates x-power of e.                     |

> ![tips](icon/tips.svg)Tips: You can click ![icon_menu](icon/icon_menu.svg) > **History** to view historical expressions and memory list, and delete history, too.

### Symbolic Fault-tolerant Computing

Calculator supports keyboard operation and fault-tolerant computing of special symbols besides normal numbers and operation symbols. The input of expressions will not be affected by the input status as well as the case state of the keyboard.

- Fault-tolerance processing of multiplication: Input asterisk (*) or letter x to trigger multiplication;
- Fault-tolerance processing of division: Input division (/)  to trigger division;
- Fault-tolerance processing of addition: Input addition (+)  to trigger addition;
- Fault-tolerance processing of subtraction: Input minus (-) or underline (_) to trigger subtraction;
- Fault-tolerance processing of percent sign: Input percent sign (%)  to trigger remainder percent sign;
- Fault-tolerance processing of decimal point: Input an English decimal point (.) or a Chinese period (。) to trigger decimal point.
- Fault-tolerance processing of the bracket symbol: Input open and close brackets to trigger bracket;
- Fault-tolerance processing of equal sign: Input = in English and Chinese or press the Enter key to trigger equal sign;
- Fault-tolerance processing of the clear symbol: Press **Esc** to trigger clearing;
- Fault-tolerance processing of the delete symbol: Press **Backspace**  to trigger deleting;

### Expression

- Historical Expression

  - Click  =  in the current expression input area or press the **Enter** key on the keyboard to perform calculation and display the calculated result in the current input box. The expression becomes historical expression. There is no limit on the number of historical expressions.
  - Reedit: Click a single expression to reedit. The expression is displayed in the expression input area. After editing, press the **Enter** key or = on the keyboard or click = in the expression input area to modify the result of historical expression and linkage expression.
  - Expression error: If the expression input is incorrect, it is unavailable to perform calculation and "Expression error" is displayed.

- Input Expression

  - Large characters show the expression currently being entered in the area.
  
  - Press **Ctrl + C** on the keyboard to copy the calculation result of the current expression to the system clipboard. If there is expression error, the current expression will be copied to the system clipboard.
  - Percentage conversion rule:

    - Percentage is treated as operation symbol and converted to number directly. For example, when you input 1%, 0.01 is displayed on the interface.  When you input another 1, the number displayed will be 0.011. Input another %, the number displayed will be 0.00011.

    - Affect digital linkage effect: The result of the former expression will be automatically displayed in the input line. The % entered will be converted to a number without linkage with the former expression. For example, if the result for the former expression is 10, the number in the input line is 10. When you input %, it will become 0.1.
  
  - Expressions with no actual operation symbol will not listed in the history record.

- Scientific Notation

  When the calculation result is more than 16 digits / 32 digits respectively, it will be displayed in scientific notation, that is, taking the first 16 digits / 32 digits multiplied by 10 to the power of plus or minus n.

  - When the calculation result is positive and greater than 16 digits / 32 digits, it is displayed in the following format: number + numbers of 15 digits / 31 digits after the decimal point+E+number.

  - When the calculation result is decimal and greater than 16 digits / 32 digits, it is displayed in the following format: number + numbers of 15 digits / 31 digits after the decimal point+E-number.

  ![0|scientific-notation](jpg/scientific-notation.png)

### Digital Linkage

- It is available to continue entering operator after an expression displays its numerical result. The first number in the expression now is the calculation result of the previous one.  Take the current expression 10 + 20 = 30 for example. The displayed result is 30.  Input + and number 9 and a new expression will be formed, 30 + 9. Press the **Enter** key and the calculation result of the new expression will be 39.
- After the two expressions are linked, modify the numbers and operators of the previous expression. If the calculation result changes, it will affect the result of the new expression linked to it. For example, the two expressions 10 + 20 = 30 and 30 +9 = 39 are linked. If the operator +  in the first expression is changed to *, the expression will be 10 x 20 = 200. The second expression will be changed to 200 + 9 = 209 automatically. Up to 9 expression linkages are supported according to this rule.
-  In the process of re-editing an expression containing linked numbers, if the expression of linked numbers or linked numbers are modified incorrectly, the linkage can be released. 

>![notes](icon/notes.svg)Notes: This function is only supported under Standard Mode.


## Main Menu

On the main menu, you can switch window themes, view help manual and get more information about Calculator.

### History 

This function is available only in Scientific Mode.
1. On the Calculator interface, click  ![main_menu](icon/main_menu.svg).
2. Click **History** to view historical expressions and memory list.

### Theme

The window theme includes Light Theme, Dark Theme and System Theme (default).

1.  On the Calculator interface, click  ![main_menu](icon/main_menu.svg).
2.  Click **Theme** to select a theme.

### Help

View Help to get more information about Calculator.

1.  On the Calculator interface, click ![main_menu](icon/main_menu.svg).
2.  Select **Help**.
3.  View the manual.

### About

1. On the Calculator interface, click ![main_menu](icon/main_menu.svg).
2. Select **About**.
3. View the version description.

### Exit

1. On the Calculator interface, click ![main_menu](icon/main_menu.svg).
2. Click **Exit** to exit.

<div class="version-info"><span>Update Date: 2020-09-22</span><span> Version: 5.5</span></div>