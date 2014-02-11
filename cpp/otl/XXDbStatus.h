//
//  XXDbStatus.h
//  XXOtl
//
//  Created by Javier Fuchs on 1/1/11.
//  Copyright (c) 2013 [Your company name here]. All rights reserved.
//

#ifndef __XXDBESTADO_H__
#define __XXDBESTADO_H__

#include <vector>
class XXString;
class XXSome;
class XXStatus;
class XXDate;
class XXDbSome;
class XXSome;
/*
CREATE TABLE status (
  id INTEGER UNSIGNED NOT NULL,
  note VARCHAR(100) NOT NULL,
  PRIMARY KEY(id),
  INDEX (id),
  UNIQUE INDEX (id),
  UNIQUE INDEX (note)
);

INSERT INTO status (id, note) VALUES (0, 'x0');
INSERT INTO status (id, note) VALUES (1, 'x1');
INSERT INTO status (id, note) VALUES (2, 'x2');
INSERT INTO status (id, note) VALUES (3, 'x3');


CREATE TABLE some_status (
  some_id INTEGER UNSIGNED NOT NULL,
  status_id INTEGER UNSIGNED NOT NULL,
  updateAt TIMESTAMP NOT NULL DEFAULT now() on update CURRENT_TIMESTAMP,
  PRIMARY KEY(some_id, status_id),
  FOREIGN KEY(some_id)
    REFERENCES some(id)
      ON DELETE NO ACTION
      ON UPDATE NO ACTION,
  FOREIGN KEY(status_id)
    REFERENCES status(id)
      ON DELETE NO ACTION
      ON UPDATE NO ACTION
);
*/





class XXDbStatus {
protected:
private:

public:
	XXDbStatus();
	~XXDbStatus();

	static bool select(XXStatus::Vector& v);
	static bool insert(XXSome& some, XXStatus& status);
	static bool update(XXSome& some, XXStatus& status, XXString& table);
};

#endif // __XXDBESTADO_H__

