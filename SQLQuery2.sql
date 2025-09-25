USE online_sales;
GO

CREATE TABLE online_store
(
	name_store VARCHAR(64) PRIMARY KEY NOT NULL,
	email VARCHAR(50) CHECK(email LIKE '_%@_%._%') NOT NULL,
	paymant_for_Delivery BIT NOT NULL,
);

CREATE TABLE product(
	product_name VARCHAR(50) NOT NULL,
	firm VARCHAR(32) NOT NULL,
	model VARCHAR(32) NOT NULL,
	technical_specifications VARCHAR(MAX)  NOT NULL,
	
);