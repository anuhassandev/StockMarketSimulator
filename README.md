# **Stock Market Simulator**

This project is a simplified simulation of how stock market orders are processed. The program reads orders from a file, processes them in sequence, matches compatible buy and sell orders based on specific rules, and displays the evolving order book and executed trades in real-time.

---

## **Features**

- **Order Book Management**: Supports **buy** and **sell** orders, sorted by priority (market orders, limit price, and time of arrival).
- **Order Matching**:
  - Matches buy and sell orders based on price compatibility and priority.
  - Supports partial execution of orders and residual order handling.
- **Real-Time Display**:
  - Displays the current order book (buy and sell orders) after processing each order.
  - Shows matched orders, executed trades, and the updated order book.
- **Customizable Input**:
  - Reads an input file containing the previous trading price and new orders.
  - Processes orders sequentially and handles incremental updates to the order book.
- **File Output**:
  - Logs all executed trades and unexecuted orders to an output file.

---

## **How It Works**

1. **Input File Format**:
   - The first line contains the previous trading price (e.g., `4.50`).
   - Each subsequent line represents an order in the format:
     ```
     [Order ID] [Order Type] [Quantity] [Limit Price (optional)]
     ```
   - Example:
     ```
     4.50
     ord001 B 100 4.25
     ord002 S 50 M
     ord003 B 75 4.50
     ord004 S 50 4.75
     ```

   - `B`: Buy order, `S`: Sell order, `M`: Market order (if no limit price is specified).

2. **Matching Orders**:
   - **Priority Rules**:
     1. Market orders have the highest priority.
     2. Higher limit price (for buying) or lower limit price (for selling).
     3. Earlier arrival time.
   - **Execution**:
     - Matches compatible buy and sell orders.
     - Calculates the execution price:
       - The limit price of the earlier order (for two limit orders).
       - The limit price of the non-market order (if one is a market order).
       - Previous transaction price (if both are market orders).
     - Handles partial execution and creates residual orders.

3. **Output**:
   - Displays the **current order book** after each order is processed.
   - Logs executed trades to an output file.

---

## **Example**

### **Input File (input1.txt)**:
```
4.50
ord001 B 100 4.25
ord002 S 50 M
ord003 B 75 4.50
ord004 S 50 4.75
```

### **Console Output**:
```
Last trading price: 4.50
Buy                       Sell
------------------------------------------
ord001    4.25  100                                
------------------------------------------

Last trading price: 4.50
Buy                       Sell
------------------------------------------
ord001    4.25  100          ord002    M     50
------------------------------------------

Last trading price: 4.50
Buy                       Sell
------------------------------------------
ord003    4.50  75          ord002    M     50
ord001    4.25  100                                
------------------------------------------

Last trading price: 4.50
Buy                       Sell
------------------------------------------
ord003    4.50  75          ord004    4.75  50
ord001    4.25  100          ord002    M     50
------------------------------------------
```

### **Output File (output.txt)**:
```
order ord002 50 shares sold at price 4.25
order ord001 50 shares purchased at price 4.25
order ord004 50 shares unexecuted
order ord001 50 shares unexecuted
order ord003 75 shares unexecuted
```

---

## **How to Run**

1. **Compile the Project**:
   Ensure you have a C++17-compatible compiler (e.g., `g++` or Visual Studio).

   ```bash
   g++ -std=c++17 -o StockMarketSimulator *.cpp
   ```

2. **Run the Program**:
   Provide the input file as a command-line argument.

   ```bash
   ./StockMarketSimulator input1.txt
   ```

3. **Check the Output**:
   - **Console Output**: Displays the updated order book after processing each order.
   - **Output File**: Logs executed trades and unexecuted orders (e.g., `output.txt`).

---

## **Code Structure**

| File                     | Description                                                             |
|--------------------------|-------------------------------------------------------------------------|
| `Order.h/.cpp`           | Defines the `Order` class, representing a single buy or sell order.     |
| `Trade.h/.cpp`           | Defines the `Trade` class for managing executed trades.                |
| `Display.h/.cpp`         | Handles real-time display of the order book.                           |
| `Match.h/.cpp`           | Implements the order-matching logic.                                   |
| `StockMarketSimulator.cpp` | Main program logic: file input, order processing, and program execution. |

---

## **Acknowledgments**
This project serves as a learning experience in:
- C++ programming (object-oriented design, STL, file I/O).
- Real-time simulation and algorithmic trading concepts.

Take a look!
