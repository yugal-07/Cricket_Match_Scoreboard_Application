# Cricket Match Scoreboard Application

A console based cricket scoreboard written in C++ as part of my **Object Oriented Programming with C++** course project.

You enter the ball by ball result and the program keeps the scoreboard updated: team total, batters on the crease, and the bowling figures of every bowler.

## Features

- Two innings match with a target chase in the second innings
- Scoreboard shows team score, wickets and overs after every over
- Batters on the crease get their runs, balls faced, 4s and 6s updated live (`*` marks the batter on strike)
- Bowler figures (overs, maidens, runs, wickets) update after every ball
- Wides and no balls handled (extra run added, ball not counted)
- Strike changes on odd runs and at the end of every over
- Maiden over detection
- Same bowler cannot bowl two overs in a row
- Result is printed at the end (win by runs / wickets, or tie)

## OOP concepts used

| Concept | Where |
|---|---|
| Classes and objects | `Player`, `Team`, `Innings` |
| Encapsulation | private helper functions inside `Innings` |
| Constructors with default arguments | `Player(string n = "")`, `Innings(...)` |
| Composition | `Team` has a vector of `Player` objects |
| Pointers to objects | `Innings` works on `Team*` for batting and bowling sides |

## Project structure

```
.
├── cricket_scoreboard.cpp
└── README.md
```

## How to run

You need a C++ compiler (g++ is fine).

```bash
g++ cricket_scoreboard.cpp -o scoreboard
./scoreboard
```

On Windows:

```bash
g++ cricket_scoreboard.cpp -o scoreboard.exe
scoreboard.exe
```

## How to use

1. Enter the number of players per team (2 to 11) and the number of overs.
2. Enter the team names and player names. Team 1 bats first.
3. Before each over, choose the bowler by number.
4. For every ball, type one of these:

| Input | Meaning |
|---|---|
| `0` to `6` | runs scored off the bat |
| `w` | wicket (batter on strike is out) |
| `wd` | wide |
| `nb` | no ball (it will ask for runs scored off the bat) |

## Sample output

```
================================================
T2  1/0   (0.1 overs)
Target: 1   Need 0 from 5 balls
------------------------------------------------
Batsman           R     B     4s   6s   Status
C                 1     1     0    0    batting
D*                0     0     0    0    batting
Extras: 0
------------------------------------------------
Bowler            O     M    R     W
A                 0.1   0    1     0
================================================
```

