If you need Korean(한국어) explanation, go to <https://sooseongcom.com/post/qm-algorithm>.

# Quine-McCluskey Algorithm Automation
This is Quine-McCluskey Algorithm automation program.

## 1\. User Manual
### 1.1. System Requirements
* OS: Windows
* Architecture: AMD64(x64, Intel64)

### 1.2. Download
1.2.1. 아래의 설치파일 중 원하는 것을 내려받아 설치합니다.(Click the link which you want.)

* [1.0.0 installer for Windows x64](https://github.com/sooseongcom/Quine-McCluskey/releases/download/v1.0.0/Quine-McCluskey-x64-1.0.0.exe)

![차단 경고](https://sooseongcom.com/assets/images/20240928/1.png)\
1.2.2. If download is blocked, click **···** and click **Keep**.

![열기 전에 신뢰할 수 있는지 확인합니다.](https://sooseongcom.com/assets/images/20240928/2.png)\
1.2.3. Click **Show more**.

![그래도 계속](https://sooseongcom.com/assets/images/20240928/3.png)\
1.2.4. Click **Keep anyway**.
1.2.5. Then the file will be downloaded. Execute it.\
1.2.6. If SmartScreen alert appears in Windows OS, click **More info** and click **Run anyway**.

### 1.3. How To Use
1.3.1. Make **input_minterm.txt** at the directory of executed file.

1.3.2. For example, *f(A, B, C, D)=Σm(0, 1, 2, 5, 7, 10, 12, 13)+Σd(3, 8, 15)*
```
4
m 0000
m 0001
m 0010
d 0011
m 0101
m 0111
d 1000
m 1010
m 1100
m 1101
d 1111
```

[Line 1]\
First line number is input bit length.

[Line 2]\
<span style="color: #ff0000">m</span> <span style="color: #0000ff">0000</span>\
<span style="color: #ff0000">The letter which seperate true minterm and don't care minterm</span> and <span style="color: #0000ff">implicant</span>.\
<span style="color: #ff0000">m means true minterm while d means don't care minterm.</span>

1.3.3. Execute program(Quine-McCluskey-Algorithm-*version*.exe).\
1.3.4. Look at **result.txt**.
```
-0-0
110-
0--1

Cost (# of transistors): 28

```

[Line 1~3]\
These terms are essential Prime Implicants. These are\
$f(A, B, C, D)=\bar{B}\bar{D}+AB\bar{C}+\bar{A}D$\
in boolean equation.

[Line 5]\
Cost is the number of transistors. Bubble pushing is considered.

## 2\. Developer Manual
### 2.1. How To Build
(추후 작성 예정)

### 2.2. Files
* **.gitignore**: Ignore x64(folder), \*.vcxproj, \*.vcxproj.filters, \*.vcxproj.user, input_minterm.txt, result.txt
* **ImpTable.cpp**: the member of ImpTable
* **ImpTable.h**: the class for implicant table
* **LICENSE**: MIT License
* **main.cpp**: main function
* **Minterm.cpp**: the member of Minterm
* **Minterm.h**: the class for minterm of implicant table
* **PetrickTable.cpp**: the member of PetrickTable
* **PetrickTable.h**: PI table for Petrick's Method
* **PITable.cpp**: the member of PITable
* **PITable.h**: the class for PI Table

### 2.3. Implicant Table Column 1 (Step 1)
Implicant Table is handled by **ImpTable** class.

![Step 1 데이터 구조](/assets/images/20251118/p1.png)\
The type of implicant table is vector.\
The type of elements of table is multimap.\
The key of multimap is the number of 1 and the value of multimap is implicant.

### 2.4. Combining variables (Step 2~3)
Step 2~3 is handled by **int ImpTable::combine(int a)** function. a is implicant table's column number. int ImpTable::combine(int a) returns 1 if combining success, while it returns 0 if combining is not run.

```cpp
//main.cpp Line 34~38
//Step 2~3
for (int i = 0; combined == 1; i++) {
    combined = imp_table->combine(i);
}
pi = imp_table->getPI();
```

Minterm's member variable ```marked```'s initial value is 0.\
When it is checked with v, marked is to be 1.\
After all combining, Minterm whose marked==1 is stared(\*) minterm.

### 2.5. Construct a PI table (Step 4)
PI table is handled by **PITable** class.

![Step 4 programming](https://sooseongcom.com/assets/images/20251118/p4.png)\
PI table is two-dimensional vector. It is defined by ```vector<vector<int>>``` and it express only white cells of upper image.

```cpp
//PITable.cpp Line 3~12
PITable::PITable(vector<string> inpPI, vector<string> inpTM) {
/**
* Row: PIs
* Column: true minterms
* 
* -1: deleted
* 0: blank
* 1: PI covers the minterm
* 2: only 1 in the column
*/
```

```cpp
//PITable.cpp Line 129~137
int piCoverTm(string inpPI, string inpTM, int length) {
	for (int i = 0; i < length; i++) {
		if (inpPI[i] != '-' && inpPI[i] != inpTM[i]) {
			return 0;
		}
	}

	return 1;
}
```
Mark 1 where the PI covers the minterm.

### 2.6. Identify essential PIs (Step 5~6)
Step 5~6 is handled by **int PITable::identifyEPI()** function.

![Step 5 Programming](https://sooseongcom.com/assets/images/20251118/p5.png)\
Find a column that has only one 1. Change it into 2.\
If that esential PI isn't in epi, push it.\
When finding a column that has only one 1, complete++.

![Step 6 Programming](https://sooseongcom.com/assets/images/20251118/p6.png)\
Mark with -1 all columns covered by the essential PI.(But 2 is kept). complete++.

int PITable::identifyEPI() returns ```tm.size() - complete```.\
If all columns are deleted, return is 0.\
If there is columns which has 1, return is more than 0 and go to Step 7.

### 2.7. Find minimum set of PIs(Petrick's Method) (Step 7)
PI table for Petrick's Method is handled by **PetrickTable** class.

![Step 7 Programming](https://sooseongcom.com/assets/images/20251118/p7.png)\
$P=(P_2+P_3)(P_3+P_4)(P_0+P_1)(P_1+P_4)$\
$P_0$ ~ $P_4$ → int p[0] ~ p[4] dynamic allocation.

* P function: ```vector<set<int*>>```
* Each factor: ```set<int*>```
* Element in the factor: int*

Find a combination of p[0] through p[4] that results in either 0 or 1. I borrowed the redundant permutation algorithm.

Among the candidates for minimum set of PIs, select the term with the least number of PIs.\
If there are several terms with the least number of PI (PetrickTable.cpp Line 179 to 190 else statements), compare the number of transistors and choose the cheapest (PetrickTable.cpp Line 195 to 231 int PetrickTable::cost(int* pterms) function).

### 2.8. Cost (# of transistors)
```
-0-0
110-
0--1
```
This is "$f(A, B, C, D)=\bar{B}\bar{D}+AB\bar{C}+\bar{A}D$ in boolean equation.\
For use NAND Gate rather than AND or OR, do bubble pushing.

![Bubble Pushing](https://sooseongcom.com/assets/images/20251118/p8.png)\
AND and OR are all replaced into NAND.\

* the number of transistor of *N*-input NAND Gate: 2*N*
* the number of transistor of NOT Gate: 2

## 3\. References
1\) novs. 2022. "[C++] STL map vs multimap (feat. multimap equal_range)", nov.Zip. (2025. 11. 28. 방문). <https://novlog.tistory.com/entry/C-STL-map-vs-multimap-feat-multimap-equalrange>\
2\) Jihan. 2023. "C++로 Quine-McCluskey 구현하기", bbbjihan.log. (2025. 11. 28. 방문). <https://velog.io/@bbbjihan/C로-Quine-McCluskey-구현하기>\
3\) SCRIPTS BY. 2023. "vector VS list 뭘 써야 하지?", SCRIPTS BY. (2025. 11. 28. 방문). <https://nx006.tistory.com/20>\
4\) BlockDMask. 2017. "[C++] multimap container 정리 및 사용법", 가면 뒤의 기록. (2025. 11. 28. 방문). <https://blockdmask.tistory.com/88>\
5\) horang. n. d. "C++ std::string::erase 정리", Code by horang. (2025. 11. 18. 방문). <https://hoho325.tistory.com/317>\
6\) BLASTIC. 2012. "[Quine-McCluskey Method] Petrick's Method", BLASTIC. (2025. 11. 18. 방문). <https://blastic.tistory.com/204>\
7\) 젠니. 2023. "[논리회로설계] Petrick's Method", Jenvelop log. (2025. 11. 18. 방문). <https://velog.io/@tmdtmdqorekf/논리회로설계-Patricks-Method>\
8\) 공부하는 식빵맘. 2020. "(C++) 중복 순열(Repeated Permutation) 구현하기", 평생 공부 블로그 : Today I Learned. (2025. 11. 18. 방문). <https://ansohxxn.github.io/algorithm/repeated-permutation/>\
9\) Song 전자공학. 2025. "CMOS Logic Gates: CMOS 논리 게이트", Song 전자공학. (2025. 11. 18. 방문). <https://blog.naver.com/songsite123/223712874895>\
10\) spring. 2020. "[c++] std::vector", springkim.log. (2025. 11. 18. 방문). <https://velog.io/@springkim/c-stdvector>