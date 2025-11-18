CREATE DATABASE IF NOT EXISTS smart_billing_db;
USE smart_billing_db;

CREATE TABLE IF NOT EXISTS bills (
  id INT AUTO_INCREMENT PRIMARY KEY,
  customerName VARCHAR(255) NOT NULL,
  totalAmount DECIMAL(10, 2) NOT NULL,
  billDate TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  status VARCHAR(50) DEFAULT 'completed'
);

CREATE TABLE IF NOT EXISTS bill_items (
  id INT AUTO_INCREMENT PRIMARY KEY,
  billId INT NOT NULL,
  itemName VARCHAR(255) NOT NULL,
  price DECIMAL(10, 2) NOT NULL,
  quantity INT NOT NULL,
  subtotal DECIMAL(10, 2) NOT NULL,
  FOREIGN KEY (billId) REFERENCES bills(id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS daily_stats (
  id INT AUTO_INCREMENT PRIMARY KEY,
  date DATE UNIQUE NOT NULL,
  totalBills INT DEFAULT 0,
  totalRevenue DECIMAL(12, 2) DEFAULT 0
);

CREATE TABLE IF NOT EXISTS device_logs (
  id INT AUTO_INCREMENT PRIMARY KEY,
  deviceId VARCHAR(50) NOT NULL,
  action VARCHAR(255) NOT NULL,
  timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE INDEX idx_billDate ON bills(billDate);
CREATE INDEX idx_billId ON bill_items(billId);
CREATE INDEX idx_statsDate ON daily_stats(date);
