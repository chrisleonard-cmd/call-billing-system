/* Call Billing System - Micro Project 07 (Telecom)
   Course : 25FP207 C Programming for Food Processing
   Features: add call, call history, monthly bill, file handling */

#include <stdio.h>
#include <string.h>

#define MAX_CALLS   100
#define FILE_NAME   "calls.txt"
#define MONTHLY_RENTAL 100.0f   /* fixed monthly rental in rupees */
#define GST_RATE       0.18f    /* 18% GST on the bill            */

/* One record of a call */
struct Call {
    char  number[15];   /* number dialled          */
    int   type;         /* 1 = Local, 2 = STD, 3 = ISD */
    int   minutes;      /* call duration in minutes */
    float charge;       /* charge for this call     */
};

struct Call calls[MAX_CALLS];   /* call history */
int count = 0;                  /* number of calls stored */

/* Return the rate per minute for a call type */
float get_rate(int type)
{
    if (type == 1) return 1.00f;      /* Local : Rs 1.00 per min */
    else if (type == 2) return 2.50f; /* STD   : Rs 2.50 per min */
    else return 8.00f;                /* ISD   : Rs 8.00 per min */
}

/* Return the name of a call type */
const char *get_type_name(int type)
{
    if (type == 1) return "Local";
    else if (type == 2) return "STD";
    else return "ISD";
}

/* Read old records from the file when the program starts */
void load_calls(void)
{
    FILE *fp = fopen(FILE_NAME, "r");
    if (fp == NULL) return;           /* no file yet */
    while (count < MAX_CALLS &&
           fscanf(fp, "%14s %d %d %f", calls[count].number,
                  &calls[count].type, &calls[count].minutes,
                  &calls[count].charge) == 4)
        count++;
    fclose(fp);
}

/* Write all records to the file */
void save_calls(void)
{
    int i;
    FILE *fp = fopen(FILE_NAME, "w");
    if (fp == NULL) {
        printf("Error: could not open file.\n");
        return;
    }
    for (i = 0; i < count; i++)
        fprintf(fp, "%s %d %d %.2f\n", calls[i].number, calls[i].type,
                calls[i].minutes, calls[i].charge);
    fclose(fp);
}

/* Option 1: add a new call and calculate its charge */
void add_call(void)
{
    struct Call c;
    if (count >= MAX_CALLS) {
        printf("Call history is full!\n");
        return;
    }
    printf("Enter phone number      : ");
    scanf("%14s", c.number);
    printf("Call type (1-Local 2-STD 3-ISD): ");
    scanf("%d", &c.type);
    if (c.type < 1 || c.type > 3) {
        printf("Invalid call type!\n");
        return;
    }
    printf("Duration in minutes     : ");
    scanf("%d", &c.minutes);
    if (c.minutes <= 0) {
        printf("Duration must be greater than 0!\n");
        return;
    }
    c.charge = c.minutes * get_rate(c.type);   /* charge = minutes x rate */
    calls[count++] = c;
    save_calls();
    printf("Call added. Charge = Rs %.2f\n", c.charge);
}

/* Option 2: show every stored call */
void view_history(void)
{
    int i;
    if (count == 0) {
        printf("No calls recorded yet.\n");
        return;
    }
    printf("\n%-4s %-15s %-7s %-8s %-10s\n", "No.", "Number", "Type", "Minutes", "Charge(Rs)");
    printf("----------------------------------------------------\n");
    for (i = 0; i < count; i++)
        printf("%-4d %-15s %-7s %-8d %-10.2f\n", i + 1, calls[i].number,
               get_type_name(calls[i].type), calls[i].minutes, calls[i].charge);
}

/* Option 3: monthly bill summary */
void monthly_bill(void)
{
    int i, total_minutes = 0;
    float call_charges = 0, gst, total;

    for (i = 0; i < count; i++) {
        total_minutes += calls[i].minutes;
        call_charges  += calls[i].charge;
    }
    gst   = (call_charges + MONTHLY_RENTAL) * GST_RATE;
    total = call_charges + MONTHLY_RENTAL + gst;

    printf("\n========== MONTHLY BILL SUMMARY ==========\n");
    printf("Total calls made    : %d\n", count);
    printf("Total minutes used  : %d\n", total_minutes);
    printf("Call charges        : Rs %.2f\n", call_charges);
    printf("Monthly rental      : Rs %.2f\n", MONTHLY_RENTAL);
    printf("GST (18%%)           : Rs %.2f\n", gst);
    printf("------------------------------------------\n");
    printf("TOTAL BILL AMOUNT   : Rs %.2f\n", total);
    printf("==========================================\n");
}

int main(void)
{
    int choice;
    load_calls();                 /* load saved records */
    do {
        printf("\n===== CALL BILLING SYSTEM =====\n");
        printf("1. Add a call\n");
        printf("2. View call history\n");
        printf("3. Generate monthly bill\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: add_call();     break;
            case 2: view_history(); break;
            case 3: monthly_bill(); break;
            case 4: printf("Thank you!\n"); break;
            default: printf("Invalid choice, try again.\n");
        }
    } while (choice != 4);
    return 0;
}
