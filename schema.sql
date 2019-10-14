CREATE TABLE Tesseramenti (
  anno INTEGER PRIMARY KEY,
  apertura TEXT NOT NULL,
  chiusura TEXT
);

CREATE TABLE Soci (
  numeroTessera INTEGER NOT NULL,
  tesseramento INTEGER NOT NULL,
  corsoDiLaurea TEXT,
  matricola TEXT NOT NULL,
  nome TEXT NOT NULL,
  cognome TEXT NOT NULL,
  professione TEXT,
  email TEXT NOT NULL,
  cellulare TEXT NOT NULL,
  cestinato INTEGER NOT NULL DEFAULT 0,
  blacklist INTEGER NOT NULL DEFAULT 0,
  FOREIGN KEY (tesseramento) REFERENCES Tesseramenti(anno),
  FOREIGN KEY (corsoDiLaurea) REFERENCES CorsiDiLaurea(id),
  PRIMARY KEY (numeroTessera, tesseramento)
);

CREATE TABLE CorsiDiLaurea (
  id TEXT PRIMARY KEY,
  nomeCorso TEXT NOT NULL,
  storico INTEGER NOT NULL DEFAULT 0
);

CREATE TABLE Admin (
  username TEXT PRIMARY KEY,
  passwordHash TEXT NOT NULL,
  canCreateUsers INTEGER NOT NULL,
  active INTEGER NOT NULL
);

CREATE TABLE Sessioni (
  auth_token TEXT PRIMARY KEY,
  creazione TEXT NOT NULL,
  scadenza TEXT NOT NULL,
  utente TEXT NOT NULL,
  FOREIGN KEY (utente) REFERENCES Admin(username)
);

CREATE TABLE Azioni (
  id TEXT PRIMARY KEY,
  moment TEXT NOT NULL,
  action TEXT NOT NULL,
  user TEXT NOT NULL,
  FOREIGN KEY (user) REFERENCES Admin(username)
);
