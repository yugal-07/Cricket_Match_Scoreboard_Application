// Cricket Match Scoreboard Application
// OOPS with C++ - college project

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Player {
public:
  string name;

  int runs, balls, fours, sixes;
  bool isOut, hasBatted;
  int ballsBowled, runsGiven, wickets, maidens;
  int runsThisOver;

  Player(string n = "") {
    name = n;
    runs = balls = fours = sixes = 0;
    isOut = hasBatted = false;
    ballsBowled = runsGiven = wickets = maidens = 0;
    runsThisOver = 0;
  }

  // called when this player faces ball
  void batRun(int r) {
    runs += r;
    balls++;
    if (r == 4)
      fours++;
    if (r == 6)
      sixes++;
  }

  void faceDot() { balls++; }

  // called when this player bowls
  void bowlBall(bool legal) {
    if (legal)
      ballsBowled++;
  }

  void giveRuns(int r) {
    runsGiven += r;
    runsThisOver += r;
  }

  string oversBowled() {
    return to_string(ballsBowled / 6) + "." + to_string(ballsBowled % 6);
  }
};

class Team {
public:
  string name;
  vector<Player> players;

  Team(string n = "") { name = n; }

  void addPlayer(string pname) { players.push_back(Player(pname)); }

  int size() { return players.size(); }
};

class Innings {
private:
  Team *bat, *bowl;
  int total, wkts, balls, maxOvers, extras;
  int striker, nonStriker, nextIn;
  int curBowler, lastBowler;
  int target;
  bool newOver;

  bool finished() {
    if (balls >= maxOvers * 6)
      return true;
    if (wkts >= bat->size() - 1)
      return true;
    if (target > 0 && total >= target)
      return true;
    return false;
  }

  void chooseBowler() {
    cout << "\nNew over! Pick a bowler from " << bowl->name << ":\n";
    for (int i = 0; i < bowl->size(); i++) {
      cout << "  " << i + 1 << ". " << bowl->players[i].name;
      if (i == lastBowler)
        cout << "  (bowled last over)";
      cout << endl;
    }
    int c;
    while (true) {
      cout << "Choice: ";
      cin >> c;
      c--;
      if (c < 0 || c >= bowl->size()) {
        cout << "Invalid number, try again.\n";
      } else if (c == lastBowler) {
        cout << "Same bowler can't bowl two overs in a row.\n";
      } else
        break;
    }
    curBowler = c;
    bowl->players[curBowler].runsThisOver = 0;
  }

  void swapStrike() {
    int t = striker;
    striker = nonStriker;
    nonStriker = t;
  }

  void bringNextBatsman() {
    if (nextIn < bat->size()) {
      striker = nextIn;
      bat->players[striker].hasBatted = true;
      nextIn++;
      cout << "New batsman: " << bat->players[striker].name << endl;
    }
  }

  void endOfOver() {
    Player &b = bowl->players[curBowler];
    if (b.runsThisOver == 0) {
      b.maidens++;
      cout << "Maiden over!\n";
    }
    lastBowler = curBowler;
    swapStrike();
    newOver = true;
    show();
  }

  // reads one ball and updates everything
  void playBall() {
    Player &bt = bat->players[striker];
    Player &bw = bowl->players[curBowler];

    cout << "\n"
         << bw.name << " to " << bt.name << "  [" << balls / 6 << "."
         << balls % 6 + 1 << "]\n";
    cout << "Enter (0-6 runs, w = wicket, wd = wide, nb = no ball): ";
    string in;
    cin >> in;

    if (in == "wd") {
      // wide - 1 extra run, ball not counted
      total += 1;
      extras += 1;
      bw.giveRuns(1);
      cout << "Wide!\n";
    } else if (in == "nb") {
      int r;
      cout << "Runs scored off the bat on no ball: ";
      cin >> r;
      total += 1 + r;
      extras += 1;
      bw.giveRuns(1 + r);
      bt.runs += r; // ball is not faced on a no ball
      if (r == 4)
        bt.fours++;
      if (r == 6)
        bt.sixes++;
      cout << "No ball!\n";
      if (r % 2 == 1)
        swapStrike();
    } else if (in == "w" || in == "W") {
      bt.faceDot();
      bt.isOut = true;
      bw.bowlBall(true);
      bw.wickets++;
      wkts++;
      balls++;
      cout << "OUT! " << bt.name << " is dismissed.\n";
      if (wkts < bat->size() - 1)
        bringNextBatsman();
      if (balls % 6 == 0)
        endOfOver();
    } else if (in.size() == 1 && in[0] >= '0' && in[0] <= '6') {
      int r = in[0] - '0';
      bt.batRun(r);
      bw.bowlBall(true);
      bw.giveRuns(r);
      total += r;
      balls++;
      if (r == 4)
        cout << "FOUR!\n";
      if (r == 6)
        cout << "SIX!\n";
      if (r % 2 == 1)
        swapStrike();
      if (balls % 6 == 0)
        endOfOver();
    } else {
      cout << "Didn't understand that, enter the ball again.\n";
    }
  }

public:
  Innings(Team *b, Team *bw, int overs, int tgt = 0) {
    bat = b;
    bowl = bw;
    maxOvers = overs;
    target = tgt;
    total = wkts = balls = extras = 0;
    striker = 0;
    nonStriker = 1;
    nextIn = 2;
    curBowler = -1;
    lastBowler = -1;
    newOver = true;
  }

  int getTotal() { return total; }
  int getWickets() { return wkts; }

  void show() {
    cout << "\n================================================\n";
    cout << bat->name << "  " << total << "/" << wkts << "   (" << balls / 6
         << "." << balls % 6 << " overs)\n";
    if (target > 0)
      cout << "Target: " << target << "   Need " << target - total << " from "
           << maxOvers * 6 - balls << " balls\n";
    cout << "------------------------------------------------\n";
    cout << left << setw(18) << "Batsman" << setw(6) << "R" << setw(6) << "B"
         << setw(5) << "4s" << setw(5) << "6s" << "Status\n";

    for (int i = 0; i < bat->size(); i++) {
      Player &p = bat->players[i];
      if (!p.hasBatted)
        continue;
      string nm = p.name;
      if (!p.isOut && i == striker)
        nm += "*"; // * = on strike
      cout << left << setw(18) << nm << setw(6) << p.runs << setw(6) << p.balls
           << setw(5) << p.fours << setw(5) << p.sixes;
      if (p.isOut)
        cout << "out";
      else
        cout << "batting";
      cout << endl;
    }
    cout << "Extras: " << extras << endl;

    cout << "------------------------------------------------\n";
    cout << left << setw(18) << "Bowler" << setw(6) << "O" << setw(5) << "M"
         << setw(6) << "R" << "W\n";
    for (int i = 0; i < bowl->size(); i++) {
      Player &p = bowl->players[i];
      if (p.ballsBowled == 0 && i != curBowler)
        continue;
      cout << left << setw(18) << p.name << setw(6) << p.oversBowled()
           << setw(5) << p.maidens << setw(6) << p.runsGiven << p.wickets
           << endl;
    }
    cout << "================================================\n";
  }

  void play() {
    bat->players[0].hasBatted = true;
    bat->players[1].hasBatted = true;

    cout << "\n--- " << bat->name << " batting ---\n";
    cout << "Openers: " << bat->players[0].name << " and "
         << bat->players[1].name << endl;

    while (!finished()) {
      if (newOver) {
        chooseBowler();
        newOver = false;
      }
      playBall();
    }
    cout << "\nInnings over!\n";
    show();
  }
};

// ---------- main ----------
Team makeTeam(int num, int n) {
  string tname;
  cout << "Enter name of team " << num << ": ";
  getline(cin, tname);
  Team t(tname);
  for (int i = 1; i <= n; i++) {
    string pn;
    cout << "  Player " << i << ": ";
    getline(cin, pn);
    t.addPlayer(pn);
  }
  return t;
}

int main() {
  int n, overs;

  cout << "===== CRICKET MATCH SCOREBOARD =====\n";
  do {
    cout << "Players per team (2 to 11): ";
    cin >> n;
  } while (n < 2 || n > 11);

  do {
    cout << "Number of overs: ";
    cin >> overs;
  } while (overs < 1);
  cin.ignore(); // clear the newline before getline

  Team t1 = makeTeam(1, n);
  cout << endl;
  Team t2 = makeTeam(2, n);

  cout << "\n" << t1.name << " will bat first.\n";

  Innings first(&t1, &t2, overs);
  first.play();

  int target = first.getTotal() + 1;
  cout << "\n" << t2.name << " need " << target << " runs to win.\n";

  Innings second(&t2, &t1, overs, target);
  second.play();

  // result
  cout << "\n********** RESULT **********\n";
  if (second.getTotal() >= target) {
    cout << t2.name << " won by " << (n - 1 - second.getWickets())
         << " wicket(s)\n";
  } else if (second.getTotal() == first.getTotal()) {
    cout << "Match tied!\n";
  } else {
    cout << t1.name << " won by " << first.getTotal() - second.getTotal()
         << " run(s)\n";
  }

  return 0;
}
