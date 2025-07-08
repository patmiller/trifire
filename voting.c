#include "voting.h"


extern char countVotes(Vote votes[], int numVotes){
    int Rcount = 0;
    int Lcount = 0;
    int Scount = 0;
    int Fcount = 0;
    for (int i = 0; i < numVotes; i++) {
        if (votes[i].vote == 'R' || votes[i].vote == 'r') {
            Rcount++;
        } else if (votes[i].vote == 'L' || votes[i].vote == 'l') {
            Lcount++;
        } else if (votes[i].vote == 'S' || votes[i].vote == 's') {
            Scount++;
        } else if (votes[i].vote == 'F' || votes[i].vote == 'f') {
            Fcount++;
        }
    }
    int maxVotes = Rcount;
    int winningOption = 0;
    int voteCounts[] = {Rcount, Lcount, Scount, Fcount};
    for (int i = 1; i < 4; i++) {
        if (voteCounts[i] > maxVotes) {
            maxVotes = voteCounts[i];
            winningOption = i;
        }
    }
    switch (winningOption) {
        case 0: return 'R'; // Right
        case 1: return 'L'; // Left
        case 2: return 'S'; // Spin
        case 3: return 'F'; // Fire
    }
}