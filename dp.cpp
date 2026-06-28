
//  MODULE 4: Dynamic Programming - 0/1 Knapsack



static const int DP_SCALE = 2; // 1 unit = 0.5 hours

static int getVal(const vector<map<int,int>>& dp, int row, int col) {
    auto it = dp[row].find(col);
    return (it != dp[row].end()) ? it->second : 0;
}

// ---- Print the DP table (only when capacity <= 20 units) ----
static void printDPTable(const vector<map<int,int>>& dp,
                         const vector<StudyTask>& tasks,
                         int capUnits) {
    int n = (int)tasks.size();

    cout << "\n  [DP Table — each cell = max importance achievable]\n";
    cout << "  Rows = tasks added one by one | Columns = time units (1 unit = 0.5 hr)\n\n";

    // Header row
    cout << "  " << left << setw(6) << "Task" << " |";
    for (int w = 0; w <= capUnits; w++)
        cout << setw(3) << w;
    cout << "\n  ------+-" << string(capUnits * 3 + 3, '-') << "\n";

    // Data rows
    for (int i = 0; i <= n; i++) {
        if (i == 0)
            cout << "  " << left << setw(6) << "(none)" << " |";
        else
            cout << "  " << left << setw(6) << tasks[i-1].id << " |";

        // Print every column, getVal returns 0 if that coordinate
        // was never stored in the map (no improvement there)
        for (int w = 0; w <= capUnits; w++)
            cout << setw(3) << getVal(dp, i, w);
        cout << "\n";
    }
}

// ---- Main DP module function ----
AlgorithmResult runDPModule(const vector<StudyTask>& tasks, double availTime) {

    if (tasks.empty()) {
        cout << "\n[!] No dataset loaded. Please load a scenario first (Option 1 or 2).\n";
        return {"Dynamic Programming", {}, 0.0, 0, 0.0};
    }

    cout << "\n========================================================\n";
    cout << "   MODULE 4: DYNAMIC PROGRAMMING — 0/1 KNAPSACK\n";
    cout << "========================================================\n";
    cout << fixed << setprecision(1);
    cout << "  Available Study Time : " << availTime << " hours\n";
    cout << "  Number of Tasks      : " << tasks.size() << "\n";

    int n        = (int)tasks.size();
    int capUnits = (int)(availTime * DP_SCALE); //converts hours too intgers
    
    //defining the map record
    vector<map<int,int>> dp(n + 1);

    // Start clock BEFORE all DP work 
    clock_t startTime = clock();

    dp[0][0] = 0;
//loops through each task
    for (int i = 1; i <= n; i++) {
        int wt  = (int)(tasks[i-1].estimated_time * DP_SCALE);
        int val = tasks[i-1].importance_score;
//handle the skip task case 
        dp[i] = dp[i-1]; //handles the skip task case 

        for (auto& [w, imp] : dp[i-1]) {
            int newW   = w + wt;
            int newImp = imp + val;
            if (newW <= capUnits) {
                if (getVal(dp, i, newW) < newImp)
                    dp[i][newW] = newImp;
            }
        }
        
    }

    //  Stop clock AFTER all DP work 
    clock_t endTime = clock();
    double elapsedMs = (double)(endTime - startTime) / CLOCKS_PER_SEC * 1000.0;


    // Print DP table (only if small enough to be readable)

if (capUnits <= 20) {
        printDPTable(dp, tasks, capUnits);
    } else {
        cout << "\n  [DP Table has " << (capUnits + 1)
             << " columns — too wide to display fully.]\n";
    }
//read final answer
int bestImp = 0;
    int bestW   = 0;
    for (auto& [w, imp] : dp[n]) {
        if (w <= capUnits && imp > bestImp) {
            bestImp = imp;
            bestW   = w;
        }
    }

    
    // Traceback: determine which tasks were selected
 
    vector<StudyTask> selected;
    int w = bestW;
    for (int i = n; i >= 1; i--) {
        if (getVal(dp, i, w) != getVal(dp, i-1, w)) {
            // Task i was included
            selected.push_back(tasks[i-1]);
            w -= (int)(tasks[i-1].estimated_time * DP_SCALE);
        }
        // If equal, task i was skipped  just move up a row
    }
    reverse(selected.begin(), selected.end()); // restore original order

    
    // Calculate totals
   
    double totalTime = 0.0;
    int    totalImp  = 0;
    vector<string> selectedIDs;
    for (const auto& t : selected) {
        totalTime += t.estimated_time;
        totalImp  += t.importance_score;
        selectedIDs.push_back(t.id);
    }

  
    // STEP 10: Print results table
    cout << "\n  -------------------------------------------------------\n";
    cout << "   SELECTED TASKS (Optimal Combination by DP)\n";
    cout << "  -------------------------------------------------------\n";
    cout << "  " << left << setw(5)  << "ID"
                         << setw(28) << "Task Name"
                         << setw(10) << "Time(hrs)"
                         << setw(12) << "Importance" << "\n";
    cout << "  " << string(55, '-') << "\n";

    if (selected.empty()) {
        cout << "  [No tasks fit within the available time budget.]\n";
    } else {
        for (const auto& t : selected) {
            cout << "  " << left << setw(5)  << t.id
                                 << setw(28) << t.name
                                 << setw(10) << t.estimated_time
                                 << setw(12) << t.importance_score << "\n";
        }
    }

    cout << "  " << string(55, '-') << "\n";
    cout << "  " << left << setw(33) << "TOTAL"
                         << setw(10) << totalTime
                         << setw(12) << totalImp  << "\n";
    cout << "  " << string(55, '-') << "\n";
    cout << "\n  Execution Time  : " << fixed << setprecision(4)
         << elapsedMs << " ms\n";
    cout << "  Time Used       : " << fixed << setprecision(1)
         << totalTime << " / " << availTime << " hours\n";
    cout << "  Time Remaining  : " << (availTime - totalTime) << " hours\n";
    cout << "\n  [DP guarantees the OPTIMAL selection no other combination\n";
    cout << "   of these tasks gives higher importance within the time limit.]\n";
    cout << "========================================================\n";

   
    return {"Dynamic Programming", selectedIDs, totalTime, totalImp, elapsedMs};
}