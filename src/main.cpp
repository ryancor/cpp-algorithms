#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>

#ifdef __unix__
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../include/kern.hpp"
#endif

#include "../include/helper.hpp"
#include "../include/requests.hpp"
#include "../include/sql.hpp"
#include "../include/main.hpp"

using namespace std;

static void exit_ITR(void) __attribute__ ((destructor));

string pick_food(int x) {
  split_items f;
  f.food_1 = split_up_item(DataGet()[x]) + ", ";
  return f.food_1;
}

double pick_price(int x) {
  split_items f;
  f.price_1 = atof(split_up_char(DataGet()[x], 2));
  return f.price_1;
}

int main(int argc, char **argv) {
  string menu_choice, ordered;
  unsigned long cust_id = customer_id();
  int food_choice, remove_choice, ordered_count = 0;
  float total = 0.00;

  // command line arguments
  #ifdef __unix__
    if(argc >= 2) {
      if(strncmp(argv[1], "help", 4) == 0) {
        std::cout << "./main <elf_file>" << std::endl;
      } else {
        check_file(argv[1]);
      }
      exit(0);
    }
  #else
    if(argc == 3) {
      if(strncmp(argv[1], "proxy", 5) == 0) {
        send_through_proxy(argv[2], "127.0.0.1", "8080");
      } else {
        std::cout << "'" << argv[1] << "'" << " is an unknown command" << std::endl;
      }
      exit(0);
    } else if(argc == 2) {
      std::cout << "./main proxy <website>" << std::endl;
      exit(0);
    }
  #endif

  #ifdef __unix__
  #include "../lib/pal.hpp"

  if(isPalindrome(user_request())) {
    std::cout << "You have a very special name ;)" << std::endl;
    printf("You earned a debug address: 0x%08x\n\n", readKey());
  }

  #endif

  show();
  char *new_name;
  #ifdef __APPLE__
    new_name = user_request();
    std::cout << new_name << std::endl;
  #else
    new_name = is_repeated(user_request());
    std::cout << new_name << std::endl;
  #endif

  (open_or_closed() == 1) ? cout << "Open: true\n" : cout << "Open: false\n";

  std::cout << "Customer ID: #" << cust_id << std::endl << std::endl;

  while(1) {
    findForeignFiles();

    std::cout << "Total Price: $" << std::fixed <<
    std::setprecision(2) << total << endl;
    std::cout << "Ordered: " << ordered << endl << endl;

    list_choices();
    getline(cin, menu_choice);

    if(menu_choice == "1") {
      cout << "Food & Drink Choices:" << std::endl;

      for(int i = 0; i < DataGet().size(); i++) {
        std::cout << i+1 << DataGet()[i] << std::endl;
      }
      cout << std::endl;
    } else if(menu_choice == "2") {
      cout << std::endl << "food>> ";
      std::cin >> food_choice;

      if((food_choice-1) < DataGet().size()) {
        std::cout << "Adding " << pick_food(food_choice-1) << " to your order." << endl;
        total += pick_price(food_choice-1);
        ordered += pick_food(food_choice-1);
        clear_cin();
      } else {
        std::cout << "Invalid Choice." << std::endl;
        clear_cin();
      }
    } else if(menu_choice == "3") {
      if(total > 0.01) {
        cout << std::endl << "remove>> ";
        cin >> remove_choice;

        if((remove_choice-1) < DataGet().size()) {
          if(ordered.find(pick_food(remove_choice-1)) != string::npos) {
            std::cout << "Removing " << pick_food(remove_choice-1) << " from your order." << endl;
            total -= pick_price(remove_choice-1);

            size_t pos = ordered.find(pick_food(remove_choice-1));
            ordered.erase(pos, pick_food(remove_choice-1).length());
            clear_cin();
          } else {
            std::cout << "You haven't ordered this item." << std::endl;
            clear_cin();
          }
        } else {
          std::cout << "Invalid Choice." << std::endl;
          clear_cin();
        }
      } else {
        std::cout << "You haven't ordered anything." << std::endl;
      }
    } else if(menu_choice == "4") {
      ordered_count++;

      if(ordered_count < 2) {
        std::cout << "Storing receipt to text file.." << std::endl << std::endl;
        receipt(cust_id, new_name, total, ordered);

        // store price in the kernel & open vma driver is sys/kernel
        #if __unix__
          // ordered is a string, and we must convert it to a char*
          set_items_to_kern(total, &ordered[0u]);
          read_sys();
        #endif
      } else {
        std::cout << "Can only store to receipt once per session!" << std::endl
                  << std::endl;
      }
    } else if(menu_choice == "5") {
      exit(-1);
    } else if(menu_choice == "6") {
      total = couponed_code(total);
    } else {
      std::cout << "Invalid Choice." << std::endl;
    }
  }
  return 0;
}

void exit_ITR(void) {
  // get price + order size from the kernel
  #if __unix__
    get_items_from_kern();
  #endif

  std::cout << "Thanks for exiting properly && interruptly!" << std::endl;
}
