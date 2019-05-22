create table SENSORRECORDS
             (  NodeID varchar(20) not null,
                recordNumber  int not null auto_increment,
                rawData varchar(100), /*remove for database assignment*/
                temperature int,
                moisture int,
                lat float(15),
                longitude float(15),
                primary key(recordNumber));

