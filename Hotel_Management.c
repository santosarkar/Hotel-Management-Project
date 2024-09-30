#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CUSTOMERS 100
#define MAX_ROOMS 50
#define MAX_NAME_LENGTH 50

typedef struct {
    int day;
    int month;
    int year;
} Date;

typedef struct {
    char name[MAX_NAME_LENGTH];
    int roomNumber;
    Date checkInDate;
    Date checkOutDate;
    int isCheckedIn;
} Customer;

Customer customers[MAX_CUSTOMERS];
int customerCount = 0;

int isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int daysInMonth(int month, int year) {
    switch (month) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            return 31;
        case 4: case 6: case 9: case 11:
            return 30;
        case 2:
            return isLeapYear(year) ? 29 : 28;
        default:
            return 0;
    }
}

int isValidDate(Date date) {
    if (date.year < 1900 || date.year > 2100)
        return 0;
    if (date.month < 1 || date.month > 12)
        return 0;
    if (date.day < 1 || date.day > daysInMonth(date.month, date.year))
        return 0;
    return 1;
}

int isDateBefore(Date d1, Date d2) {
    if (d1.year < d2.year) return 1;
    if (d1.year > d2.year) return 0;
    if (d1.month < d2.month) return 1;
    if (d1.month > d2.month) return 0;
    return d1.day < d2.day;
}

int isRoomAvailable(int roomNumber, Date checkInDate, Date checkOutDate) {
    for (int i = 0; i < customerCount; i++) {
        if (customers[i].roomNumber == roomNumber) {
            if (!(isDateBefore(checkOutDate, customers[i].checkInDate) || isDateBefore(customers[i].checkOutDate, checkInDate))) {
                return 0;
            }
        }
    }
    return 1;
}

int getValidRoomNumber() {
    int roomNumber;
    while (1) {
        printf("Enter room number (1-%d): ", MAX_ROOMS);
        if (scanf("%d", &roomNumber) != 1 || roomNumber < 1 || roomNumber > MAX_ROOMS) {
            printf("Invalid input. Please enter a valid room number between 1 and %d.\n", MAX_ROOMS);
            while (getchar() != '\n'); // Clear buffer
        } else {
            return roomNumber;
        }
    }
}

void displayBookings() {
    if (customerCount == 0) {
        printf("No bookings available.\n");
        return;
    }

    printf("Current Bookings:\n");
    for (int i = 0; i < customerCount; i++) {
        printf("Customer: %s, Room: %d, Check-in: %d/%d/%d, Check-out: %d/%d/%d, Checked-in: %s\n",
               customers[i].name, customers[i].roomNumber,
               customers[i].checkInDate.day, customers[i].checkInDate.month, customers[i].checkInDate.year,
               customers[i].checkOutDate.day, customers[i].checkOutDate.month, customers[i].checkOutDate.year,
               customers[i].isCheckedIn ? "Yes" : "No");
    }
}

void cancelBooking() {
    if (customerCount == 0) {
        printf("No bookings available to cancel.\n");
        return;
    }

    int roomNumber = getValidRoomNumber();

    for (int i = 0; i < customerCount; i++) {
        if (customers[i].roomNumber == roomNumber) {
            // Shift all subsequent customers to "remove" the canceled one
            for (int j = i; j < customerCount - 1; j++) {
                customers[j] = customers[j + 1];
            }
            customerCount--; // Decrease the customer count
            printf("Booking for room %d has been canceled.\n", roomNumber);
            return;
        }
    }
    printf("No booking found for room %d.\n", roomNumber);
}

void Check_In() {
    if (customerCount == 0) {
        printf("No bookings available for check-in.\n");
        return;
    }

    int roomNumber = getValidRoomNumber();

    for (int i = 0; i < customerCount; i++) {
        if (customers[i].roomNumber == roomNumber) {
            if (customers[i].isCheckedIn) {
                printf("Customer %s is already checked in.\n", customers[i].name);
            } else {
                customers[i].isCheckedIn = 1;
                printf("Check-in successful for %s in room number %d.\n", customers[i].name, customers[i].roomNumber);
            }
            return;
        }
    }
    printf("No booking found for the given room number.\n");
}

void Check_Out() {
    if (customerCount == 0) {
        printf("No bookings available for check-out.\n");
        return;
    }

    int roomNumber = getValidRoomNumber();

    for (int i = 0; i < customerCount; i++) {
        if (customers[i].roomNumber == roomNumber && customers[i].isCheckedIn) {
            customers[i].isCheckedIn = 0;
            printf("Check-out successful for %s from room number %d.\n", customers[i].name, customers[i].roomNumber);
            return;
        }
    }

    printf("No checked-in customer found for the given room number.\n");
}

void Room_Booking() {
    if (customerCount >= MAX_CUSTOMERS) {
        printf("Customer limit reached. Cannot book more rooms.\n");
        return;
    }

    Customer newCustomer;
    printf("\nEnter customer name: ");
    scanf("%49s", newCustomer.name);

    newCustomer.roomNumber = getValidRoomNumber();

    do {
        printf("Enter check-in date (dd mm yyyy): ");
        scanf("%d %d %d", &newCustomer.checkInDate.day, &newCustomer.checkInDate.month, &newCustomer.checkInDate.year);
        if (!isValidDate(newCustomer.checkInDate)) {
            printf("Invalid check-in date. Please try again.\n");
        }
    } while (!isValidDate(newCustomer.checkInDate));

    do {
        printf("Enter check-out date (dd mm yyyy): ");
        scanf("%d %d %d", &newCustomer.checkOutDate.day, &newCustomer.checkOutDate.month, &newCustomer.checkOutDate.year);
        if (!isValidDate(newCustomer.checkOutDate)) {
            printf("Invalid check-out date. Please try again.\n");
        } else if (!isDateBefore(newCustomer.checkInDate, newCustomer.checkOutDate)) {
            printf("Check-out date must be after the check-in date. Please try again.\n");
        }
    } while (!isValidDate(newCustomer.checkOutDate) || !isDateBefore(newCustomer.checkInDate, newCustomer.checkOutDate));

    if (!isRoomAvailable(newCustomer.roomNumber, newCustomer.checkInDate, newCustomer.checkOutDate)) {
        printf("Room is not available for the given dates.\n");
        return;
    }

    newCustomer.isCheckedIn = 0;
    customers[customerCount++] = newCustomer;
    printf("Room booked successfully.\n");
}

void Room_Avlbt() {
    int roomNumber;
    Date checkInDate, checkOutDate;

    printf("\nEnter room number: ");
    roomNumber = getValidRoomNumber();

    do {
        printf("Enter check-in date (dd mm yyyy): ");
        scanf("%d %d %d", &checkInDate.day, &checkInDate.month, &checkInDate.year);
        if (!isValidDate(checkInDate)) {
            printf("Invalid check-in date. Please try again.\n");
        }
    } while (!isValidDate(checkInDate));

    do {
        printf("Enter check-out date (dd mm yyyy): ");
        scanf("%d %d %d", &checkOutDate.day, &checkOutDate.month, &checkOutDate.year);
        if (!isValidDate(checkOutDate)) {
            printf("Invalid check-out date. Please try again.\n");
        } else if (!isDateBefore(checkInDate, checkOutDate)) {
            printf("Check-out date must be after the check-in date. Please try again.\n");
        }
    } while (!isValidDate(checkOutDate) || !isDateBefore(checkInDate, checkOutDate));

    if (isRoomAvailable(roomNumber, checkInDate, checkOutDate)) {
        printf("Room is available for the given dates.\n");
    } else {
        printf("Room is not available for the given dates.\n");
    }
}

void Service() {
    int x;

    while (1) {
        printf("\n--------Service Menu--------\n");
        printf("1. Room Service\n");
        printf("2. Laundry Service\n");
        printf("3. Free Wi-Fi\n");
        printf("4. 24/7 Customer Service\n");
        printf("5. Return to Main Menu\n");
        printf("6. Exit\n");

        printf("Enter Your Choice: ");
        scanf("%d", &x);

        switch (x) {
            case 1:
            case 2:
            case 3:
            case 4:
                printf("Available\n");
                break;
            case 5:
                return;
            case 6:
                printf("Thank You!\n");
                exit(0);
                break;
            default:
                printf("Invalid input. Please try again.\n");
        }
    }
}

void HotelInfo() {
    printf("\n*************-Vintage Villa-************** \n\n");
    printf("'Vintage Villa offers luxurious accommodations with 3-star, 5-star, and 7-star options.\n"
           "All rooms come with complimentary Wi-Fi, and we provide 24/7 room service for your convenience.\n"
           "Experience a blend of classic charm and modern amenities during your stay with us.'\n");
    printf("\n******************************************** \n");
}

int main() {
    int choice;

    while (1) {
        printf("\n-------------- Main Menu --------------\n");
        printf("1. Hotel Information\n");
        printf("2. Room Booking\n");
        printf("3. Room Availability\n");
        printf("4. Check In\n");
        printf("5. Check Out\n");
        printf("6. Cancel Booking\n");
        printf("7. Display All Bookings\n");
        printf("8. Services\n");
        printf("9. Exit\n");

        printf("Enter Your Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                HotelInfo();
                break;
            case 2:
                Room_Booking();
                break;
            case 3:
                Room_Avlbt();
                break;
            case 4:
                Check_In();
                break;
            case 5:
                Check_Out();
                break;
            case 6:
                cancelBooking();
                break;
            case 7:
                displayBookings();
                break;
            case 8:
                Service();
                break;
            case 9:
                printf("Thank You!\n");
                exit(0);
                break;
            default:
                printf("Invalid input. Please try again.\n");
        }
    }

    return 0;
}
