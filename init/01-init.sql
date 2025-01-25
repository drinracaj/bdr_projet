CREATE TABLE IF NOT EXISTS Building (
    id SERIAL PRIMARY KEY,
    street VARCHAR(50) NOT NULL,
    no VARCHAR(10) NOT NULL,
    npa INTEGER NOT NULL,
    city VARCHAR(50) NOT NULL,
    CONSTRAINT CK_Building_npa CHECK (npa >= 1000 AND npa <= 9999)
);

CREATE TABLE IF NOT EXISTS Flat (
    id SERIAL PRIMARY KEY,
    no VARCHAR(10) NOT NULL,
    idBuilding INTEGER NOT NULL,
    CONSTRAINT FK_Flat_idBuilding FOREIGN KEY (idBuilding) REFERENCES Building(id) ON DELETE RESTRICT
);

CREATE TABLE IF NOT EXISTS RoomType(
    id SERIAL PRIMARY KEY,
    type VARCHAR(30) NOT NULL,
    coefficient REAL NOT NULL,
    CONSTRAINT CK_RoomType_coefficient CHECK (coefficient IN (0, 0.5, 1))
);

CREATE TABLE IF NOT EXISTS RoomType_Flat(
    id serial PRIMARY KEY,
    idRoomType INTEGER NOT NULL,
    idFlat INTEGER NOT NULL,
    volume REAL NOT NULL,
    CONSTRAINT FK_Flat_RoomType_idRoomType FOREIGN KEY(idRoomType) REFERENCES RoomType(id) ON DELETE CASCADE,
    CONSTRAINT FK_Flat_RoomType_idFlat FOREIGN KEY(idFlat) REFERENCES Flat(id) ON DELETE CASCADE,
    CONSTRAINT CK_Flat_RoomType_volume CHECK (volume > 0)
);

CREATE TABLE IF NOT EXISTS WaterMeter (
    id serial PRIMARY KEY,
    no VARCHAR(10) NOT NULL
);

CREATE TABLE IF NOT EXISTS Flat_WaterMeter (
    id serial PRIMARY KEY,
    idWaterMeter INTEGER NOT NULL,
    idFlat INTEGER NOT NULL,
    CONSTRAINT Flat_WaterMeter_idWaterMeter FOREIGN KEY(idWaterMeter) REFERENCES WaterMeter(id) ON DELETE CASCADE,
    CONSTRAINT Flat_WaterMeter_idFlat FOREIGN KEY(idFlat) REFERENCES Flat(id) ON DELETE CASCADE,
    CONSTRAINT UC_Flat_WaterMeter_idWaterMeter_idFlat UNIQUE (idWaterMeter, idFlat)
);

CREATE TABLE IF NOT EXISTS WaterMeasurement(
    id serial PRIMARY KEY,
    idWaterMeter INTEGER NOT NULL,
    year INTEGER NOT NULL,
    measure REAL NOT NULL,
    CONSTRAINT WaterMeasurement_idWaterMeter FOREIGN KEY(idWaterMeter) REFERENCES WaterMeter(id) ON DELETE CASCADE,
    CONSTRAINT CK_WaterMeasurement_year CHECK (year < EXTRACT(YEAR FROM CURRENT_DATE)),
    CONSTRAINT CK_WaterMeasurement_measure CHECK (measure >= 0)
);

CREATE TABLE IF NOT EXISTS WaterPrice(
    id serial PRIMARY KEY,
    year INTEGER NOT NULL,
    price REAL NOT NULL,
    CONSTRAINT CK_WaterPrice_year CHECK (year < EXTRACT(YEAR FROM CURRENT_DATE)),
    CONSTRAINT CK_WaterPrice_price CHECK (price >= 0),
    CONSTRAINT UC_WaterPrice_year UNIQUE (year)
);

CREATE TABLE IF NOT EXISTS Occupancy(
    id serial PRIMARY KEY,
    idFlat INTEGER NOT NULL,
    startDate DATE NOT NULL,
    endDate DATE,
    rent REAL NOT NULL,
    fees REAL NOT NULL,
    ddm INTEGER NOT NULL,
    CONSTRAINT Occupancy_idFlat FOREIGN KEY(idFlat) REFERENCES Flat(id) ON DELETE RESTRICT,
    CONSTRAINT CK_Occupany_end CHECK (endDate > startDate),
    CONSTRAINT CK_Occupany_ddm CHECK (ddm >= 1 AND ddm <= 31)
);

CREATE TABLE IF NOT EXISTS PaymentType(
    id serial PRIMARY KEY,
    type VARCHAR(50) NOT NULL,
    CONSTRAINT UC_PaymentType_type UNIQUE (type)
);

CREATE TABLE IF NOT EXISTS Payment(
    id serial PRIMARY KEY,
    idOccupancy INTEGER NOT NULL,
    idPaymentType INTEGER NOT NULL,
    date DATE NOT NULL,
    amount REAL NOT NULL,
    CONSTRAINT Payment_idOccupancy FOREIGN KEY(idOccupancy) REFERENCES Occupancy(id) ON DELETE CASCADE,
    CONSTRAINT Payment_idPaymentType FOREIGN KEY(idPaymentType) REFERENCES PaymentType(id) ON DELETE RESTRICT,
    CONSTRAINT CK_Payment_date CHECK (date <= CURRENT_DATE),
    CONSTRAINT CK_Payment_amount CHECK (amount >= 0)
);

CREATE TABLE IF NOT EXISTS Tenant(
    id serial PRIMARY KEY,
    firstName VARCHAR(50) NOT NULL,
    lastName VARCHAR(70) NOT NULL,
    email VARCHAR(320) NOT NULL,
    tel VARCHAR(11) NOT NULL,
    CONSTRAINT UC_Tenant_email UNIQUE (email)
    CONSTRAINT UC_Tenant_tel UNIQUE (tel)
);

CREATE TABLE IF NOT EXISTS Occupancy_Tenant(
    id serial PRIMARY KEY,
    idTenant INTEGER NOT NULL,
    idOccupancy INTEGER NOT NULL,
    CONSTRAINT Occupancy_Tenant_idTenant FOREIGN KEY(idTenant) REFERENCES Tenant(id) ON DELETE CASCADE,
    CONSTRAINT Occupancy_Tenant_idOccupancy FOREIGN KEY(idOccupancy) REFERENCES Occupancy(id) ON DELETE CASCADE,
    CONSTRAINT UC_Occupancy_Tenant_idTenant_idOccupancy UNIQUE (idTenant, idOccupancy)
);

CREATE TABLE IF NOT EXISTS Company(
    id serial PRIMARY KEY,
    name VARCHAR(35) NOT NULL,
    email VARCHAR(320),
    tel VARCHAR(11)
);

CREATE TABLE IF NOT EXISTS FeeType(
    id serial PRIMARY KEY,
    type VARCHAR(50) NOT NULL,
    CONSTRAINT UC_FeeType_type UNIQUE (type)
);

CREATE TABLE IF NOT EXISTS Invoice(
    id serial PRIMARY KEY,
    idBuilding INTEGER NOT NULL,
    idCompany INTEGER NOT NULL,
    idFeeType INTEGER NOT NULL,
    amount REAL NOT NULL,
    date DATE NOT NULL,
    CONSTRAINT Invoice_idBuilding FOREIGN KEY(idBuilding) REFERENCES Building(id) ON DELETE RESTRICT,
    CONSTRAINT Invoice_idCompany FOREIGN KEY(idCompany) REFERENCES Company(id) ON DELETE RESTRICT,
    CONSTRAINT Invoice_idFeeType FOREIGN KEY(idFeeType) REFERENCES FeeType(id) ON DELETE RESTRICT,
    CONSTRAINT CK_Invoice_amount CHECK (amount >= 0)
);

CREATE TABLE IF NOT EXISTS HeatMonthlyCoefficient(
    id serial PRIMARY KEY,
    month INTEGER NOT NULL,
    coefficient REAL NOT NULL,
    CONSTRAINT UC_HeatMonthlyCoefficient_month UNIQUE (month),
    CONSTRAINT CK_HeatMonthlyCoefficient_month CHECK (month >= 1 AND month <= 12),
    CONSTRAINT CK_HeatMonthlyCoefficient_coefficient CHECK (coefficient >= 0 AND coefficient <= 100.00)
);


CREATE OR REPLACE FUNCTION checkCoefficientSum() RETURNS TRIGGER
LANGUAGE plpgsql
AS $$
BEGIN
    IF (SELECT SUM(coefficient) FROM HeatMonthlyCoefficient) > 100.00 + 0.001
    THEN RAISE EXCEPTION 'Sum of coefficient cannot exceed 100.00';
END IF;
RETURN NEW;
END $$;

CREATE TRIGGER triggerCheckCoefficientSum
BEFORE INSERT OR UPDATE ON HeatMonthlyCoefficient
FOR EACH ROW EXECUTE FUNCTION checkCoefficientSum();


-- INSERT DATA

-- Building
INSERT INTO Building (street, no, npa, city) VALUES ('Rue des Greniers', '1-3', 1522, 'Lucens');

-- Flat
INSERT INTO Flat (no, idBuilding) VALUES ('01', 1);
INSERT INTO Flat (no, idBuilding) VALUES ('11', 1);
INSERT INTO Flat (no, idBuilding) VALUES ('101', 1);

-- RoomType
INSERT INTO RoomType (type, coefficient) VALUES ('Living room', 1);
INSERT INTO RoomType (type, coefficient) VALUES ('Bedroom', 1);
INSERT INTO RoomType (type, coefficient) VALUES ('Bedroom/Living room', 1);
INSERT INTO RoomType (type, coefficient) VALUES ('Kitchen', 0.5);
INSERT INTO RoomType (type, coefficient) VALUES ('Bathroom', 0);

-- RoomType_Flat
INSERT INTO RoomType_Flat (idRoomType, idFlat, volume) VALUES (3, 1, 50);
INSERT INTO RoomType_Flat (idRoomType, idFlat, volume) VALUES (4, 1, 35);
INSERT INTO RoomType_Flat (idRoomType, idFlat, volume) VALUES (5, 1, 28);

INSERT INTO RoomType_Flat (idRoomType, idFlat, volume) VALUES (1, 2, 60);
INSERT INTO RoomType_Flat (idRoomType, idFlat, volume) VALUES (2, 2, 45);
INSERT INTO RoomType_Flat (idRoomType, idFlat, volume) VALUES (2, 2, 25);
INSERT INTO RoomType_Flat (idRoomType, idFlat, volume) VALUES (2, 2, 25);
INSERT INTO RoomType_Flat (idRoomType, idFlat, volume) VALUES (4, 2, 40);
INSERT INTO RoomType_Flat (idRoomType, idFlat, volume) VALUES (5, 2, 16);

INSERT INTO RoomType_Flat (idRoomType, idFlat, volume) VALUES (1, 3, 90);
INSERT INTO RoomType_Flat (idRoomType, idFlat, volume) VALUES (2, 3, 60);
INSERT INTO RoomType_Flat (idRoomType, idFlat, volume) VALUES (2, 3, 40);
INSERT INTO RoomType_Flat (idRoomType, idFlat, volume) VALUES (2, 3, 30);
INSERT INTO RoomType_Flat (idRoomType, idFlat, volume) VALUES (2, 3, 30);
INSERT INTO RoomType_Flat (idRoomType, idFlat, volume) VALUES (5, 3, 23);
INSERT INTO RoomType_Flat (idRoomType, idFlat, volume) VALUES (5, 3, 23);
INSERT INTO RoomType_Flat (idRoomType, idFlat, volume) VALUES (4, 3, 40);
INSERT INTO RoomType_Flat (idRoomType, idFlat, volume) VALUES (4, 3, 40);

-- WaterMeter
INSERT INTO WaterMeter (no) VALUES ('A123');
INSERT INTO WaterMeter (no) VALUES ('A456');

-- Flat_WaterMeter
INSERT INTO Flat_WaterMeter (idWaterMeter, idFlat) VALUES (1, 1);
INSERT INTO Flat_WaterMeter (idWaterMeter, idFlat) VALUES (1, 2);
INSERT INTO Flat_WaterMeter (idWaterMeter, idFlat) VALUES (1, 3);
INSERT INTO Flat_WaterMeter (idWaterMeter, idFlat) VALUES (2, 3);

-- WaterMeasurement
INSERT INTO WaterMeasurement (idWaterMeter, year, measure) VALUES (1, 2023, 911);
INSERT INTO WaterMeasurement (idWaterMeter, year, measure) VALUES (2, 2023, 50);

-- Occupancy
INSERT INTO Occupancy (idFlat, startDate, endDate, rent, fees, ddm)
VALUES (1, '2023-07-01', NULL, 850, 100, 7);
INSERT INTO Occupancy (idFlat, startDate, endDate, rent, fees, ddm)
VALUES (2, '2023-01-01', NULL, 1450, 250, 7);
INSERT INTO Occupancy (idFlat, startDate, endDate, rent, fees, ddm)
VALUES (3, '2024-01-01', NULL, 2200, 300, 7);

-- PaymentType
INSERT INTO PaymentType (type) VALUES ('Bank transfer');
INSERT INTO PaymentType (type) VALUES ('Poste');
INSERT INTO PaymentType (type) VALUES ('TWINT');

-- Payment
INSERT INTO Payment (idOccupancy, idPaymentType, date, amount) VALUES
(1, 1, '2023-07-07', 950),
(1, 1, '2023-08-07', 950),
(1, 1, '2023-09-07', 950),
(1, 2, '2023-10-07', 950),
(1, 1, '2023-11-07', 950),
(1, 1, '2023-12-07', 950),
(1, 3, '2024-01-07', 950);

INSERT INTO Payment (idOccupancy, idPaymentType, date, amount) VALUES
(2, 1, '2023-01-07', 1700),
(2, 1, '2023-02-07', 1700),
(2, 2, '2023-03-07', 1700),
(2, 1, '2023-04-07', 1700),
(2, 1, '2023-05-07', 1700),
(2, 1, '2023-06-07', 1700),
(2, 3, '2023-07-07', 1700),
(2, 1, '2023-08-07', 1700),
(2, 1, '2023-09-07', 1700),
(2, 1, '2023-10-07', 1700),
(2, 2, '2023-11-07', 1700),
(2, 1, '2023-12-07', 1700),
(2, 1, '2024-01-07', 1700);

INSERT INTO Payment (idOccupancy, idPaymentType, date, amount)
VALUES (3, 1, '2024-01-07', 2500);

-- Tenant
INSERT INTO Tenant (firstName, lastName, email, tel) VALUES
('Sarah', 'Martinez', 'sarah.martinez@email.com', '0791234567'),
('Thomas', 'Müller', 'thomas.mueller@email.com', '0789876543'),
('Marie', 'Dubois', 'marie.dubois@email.com', '0781122334');

-- Occupancy_Tenant
INSERT INTO Occupancy_Tenant (idTenant, idOccupancy) VALUES
(1, 1),
(2, 2),
(3, 3);

-- Company
INSERT INTO Company (name, email, tel) VALUES ('Energiapro SA', 'client@energiapro.ch', '0800429429');
INSERT INTO Company (name, email, tel) VALUES ('JM MICHOUD', 'michoud.ramoneur@bluewin.ch', '0794257347');
INSERT INTO Company (name, email, tel) VALUES ('groupe e', 'finance@entretec.ch', '0264667080');
INSERT INTO Company (name, email, tel) VALUES ('Pillonel Thermique', 'yp@pillonelthermique.ch', '0266635680');
INSERT INTO Company (name, email, tel) VALUES ('Commune Lucens', 'bourse@lucens.ch', '0219061559');

-- FeeType
INSERT INTO FeeType (type) VALUES ('Heating/Hot water');
INSERT INTO FeeType (type) VALUES ('Water purification');
INSERT INTO FeeType (type) VALUES ('Misc');

-- Invoice
INSERT INTO Invoice (idBuilding, idCompany, idFeeType, amount, date) VALUES
(1, 1, 1, 2450.00, '2023-03-15'),
(1, 1, 1, 1850.00, '2023-06-15'),
(1, 1, 1, 1250.00, '2023-09-15'),
(1, 1, 1, 2850.00, '2023-12-15'),
(1, 1, 1, 2950.00, '2024-03-15'),
(1, 1, 1, 2150.00, '2024-06-15');

INSERT INTO Invoice (idBuilding, idCompany, idFeeType, amount, date) VALUES
(1, 5, 2, 780.00, '2023-06-30'),
(1, 5, 2, 845.00, '2023-12-31'),
(1, 5, 2, 890.00, '2024-06-30');

INSERT INTO Invoice (idBuilding, idCompany, idFeeType, amount, date) VALUES
(1, 2, 1, 450.00, '2023-05-20'),
(1, 2, 1, 450.00, '2024-05-20');

INSERT INTO Invoice (idBuilding, idCompany, idFeeType, amount, date) VALUES
(1, 4, 1, 850.00, '2023-08-10'),
(1, 4, 1, 875.00, '2024-08-10');

INSERT INTO HeatMonthlyCoefficient (month, coefficient) VALUES
(1, 18.00),
(2, 15.00),
(3, 14.00),
(4, 8.00),
(5, 4.00),
(6, 0.0),
(7, 0.0),
(8, 0.0),
(9, 2.00),
(10, 8.00),
(11, 13.00),
(12, 18.00);

INSERT INTO WaterPrice (year, price) VALUES (2023,3.50);