#ifndef LEDGER_H_INCLUDED
#define LEDGER_H_INCLUDED

struct LedgerEntry {
    char* transactionType;
    char* transactionDetails;
    struct LedgerEntry* next;
};

struct Ledger {
    struct LedgerEntry* head;
};

void addLedgerEntry(struct Ledger* ledger, const char* transactionType, const char* transactionDetails);
void viewLedger(struct Ledger* ledger);
void initializeLedger();

#endif
