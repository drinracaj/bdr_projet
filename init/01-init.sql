--
-- PostgreSQL database dump
--

-- Dumped from database version 16.6 (Debian 16.6-1.pgdg120+1)
-- Dumped by pg_dump version 16.6 (Debian 16.6-1.pgdg120+1)

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- Name: building; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.building (
                                 id integer NOT NULL,
                                 street character varying(50) NOT NULL,
                                 no character varying(10) NOT NULL,
                                 npa integer NOT NULL,
                                 city character varying(50) NOT NULL
);


ALTER TABLE public.building OWNER TO postgres;

--
-- Name: building_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.building_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.building_id_seq OWNER TO postgres;

--
-- Name: building_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.building_id_seq OWNED BY public.building.id;


--
-- Name: company; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.company (
                                id integer NOT NULL,
                                name character varying(35) NOT NULL,
                                email character varying(320) NOT NULL,
                                tel character varying(11) NOT NULL
);


ALTER TABLE public.company OWNER TO postgres;

--
-- Name: company_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.company_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.company_id_seq OWNER TO postgres;

--
-- Name: company_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.company_id_seq OWNED BY public.company.id;


--
-- Name: feetype; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.feetype (
                                id integer NOT NULL,
                                type character varying(30) NOT NULL
);


ALTER TABLE public.feetype OWNER TO postgres;

--
-- Name: feetype_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.feetype_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.feetype_id_seq OWNER TO postgres;

--
-- Name: feetype_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.feetype_id_seq OWNED BY public.feetype.id;


--
-- Name: flat; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.flat (
                             id integer NOT NULL,
                             no character varying(10) NOT NULL,
                             building_id integer NOT NULL
);


ALTER TABLE public.flat OWNER TO postgres;

--
-- Name: flat_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.flat_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.flat_id_seq OWNER TO postgres;

--
-- Name: flat_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.flat_id_seq OWNED BY public.flat.id;


--
-- Name: heatmonthlycoefficient; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.heatmonthlycoefficient (
                                               id integer NOT NULL,
                                               month integer NOT NULL,
                                               coefficient real NOT NULL
);


ALTER TABLE public.heatmonthlycoefficient OWNER TO postgres;

--
-- Name: heatmonthlycoefficient_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.heatmonthlycoefficient_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.heatmonthlycoefficient_id_seq OWNER TO postgres;

--
-- Name: heatmonthlycoefficient_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.heatmonthlycoefficient_id_seq OWNED BY public.heatmonthlycoefficient.id;


--
-- Name: invoice; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.invoice (
                                id integer NOT NULL,
                                amount real NOT NULL,
                                date date NOT NULL,
                                building_id integer NOT NULL,
                                fee_type_id integer NOT NULL,
                                company_id integer NOT NULL
);


ALTER TABLE public.invoice OWNER TO postgres;

--
-- Name: invoice_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.invoice_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.invoice_id_seq OWNER TO postgres;

--
-- Name: invoice_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.invoice_id_seq OWNED BY public.invoice.id;


--
-- Name: occtenant; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.occtenant (
                                  id integer NOT NULL,
                                  occupancy_id integer NOT NULL,
                                  tenant_id integer NOT NULL
);


ALTER TABLE public.occtenant OWNER TO postgres;

--
-- Name: occtenant_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.occtenant_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.occtenant_id_seq OWNER TO postgres;

--
-- Name: occtenant_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.occtenant_id_seq OWNED BY public.occtenant.id;


--
-- Name: occupancy; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.occupancy (
                                  id integer NOT NULL,
                                  start date NOT NULL,
                                  rent integer NOT NULL,
                                  fees integer NOT NULL,
                                  ddm integer NOT NULL,
                                  flat_id integer NOT NULL,
                                  end_date date
);


ALTER TABLE public.occupancy OWNER TO postgres;

--
-- Name: occupancy_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.occupancy_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.occupancy_id_seq OWNER TO postgres;

--
-- Name: occupancy_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.occupancy_id_seq OWNED BY public.occupancy.id;


--
-- Name: payment; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.payment (
                                id integer NOT NULL,
                                date date NOT NULL,
                                amount integer NOT NULL,
                                paymenttype_id integer NOT NULL,
                                occupancy_id integer NOT NULL
);


ALTER TABLE public.payment OWNER TO postgres;

--
-- Name: payment_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.payment_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.payment_id_seq OWNER TO postgres;

--
-- Name: payment_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.payment_id_seq OWNED BY public.payment.id;


--
-- Name: paymenttype; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.paymenttype (
                                    id integer NOT NULL,
                                    type character varying(15) NOT NULL
);


ALTER TABLE public.paymenttype OWNER TO postgres;

--
-- Name: paymenttype_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.paymenttype_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.paymenttype_id_seq OWNER TO postgres;

--
-- Name: paymenttype_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.paymenttype_id_seq OWNED BY public.paymenttype.id;


--
-- Name: roomtype; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.roomtype (
                                 id integer NOT NULL,
                                 coefficient real NOT NULL,
                                 type character varying(20) NOT NULL
);


ALTER TABLE public.roomtype OWNER TO postgres;

--
-- Name: roomtype_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.roomtype_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.roomtype_id_seq OWNER TO postgres;

--
-- Name: roomtype_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.roomtype_id_seq OWNED BY public.roomtype.id;


--
-- Name: roomtypeflat; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.roomtypeflat (
                                     id integer NOT NULL,
                                     volume real NOT NULL,
                                     room_type_id integer NOT NULL,
                                     flat_id integer NOT NULL
);


ALTER TABLE public.roomtypeflat OWNER TO postgres;

--
-- Name: roomtypeflat_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.roomtypeflat_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.roomtypeflat_id_seq OWNER TO postgres;

--
-- Name: roomtypeflat_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.roomtypeflat_id_seq OWNED BY public.roomtypeflat.id;


--
-- Name: tenant; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.tenant (
                               id integer NOT NULL,
                               firstname character varying(35) NOT NULL,
                               lastname character varying(35) NOT NULL,
                               email character varying(320) NOT NULL,
                               tel character varying(11) NOT NULL
);


ALTER TABLE public.tenant OWNER TO postgres;

--
-- Name: tenant_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.tenant_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.tenant_id_seq OWNER TO postgres;

--
-- Name: tenant_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.tenant_id_seq OWNED BY public.tenant.id;


--
-- Name: watermeasurement; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.watermeasurement (
                                         id integer NOT NULL,
                                         year integer NOT NULL,
                                         measure real NOT NULL,
                                         watermeter_id integer NOT NULL
);


ALTER TABLE public.watermeasurement OWNER TO postgres;

--
-- Name: watermeasurement_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.watermeasurement_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.watermeasurement_id_seq OWNER TO postgres;

--
-- Name: watermeasurement_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.watermeasurement_id_seq OWNED BY public.watermeasurement.id;


--
-- Name: watermeter; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.watermeter (
                                   id integer NOT NULL,
                                   no character varying(10) NOT NULL
);


ALTER TABLE public.watermeter OWNER TO postgres;

--
-- Name: watermeter_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.watermeter_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.watermeter_id_seq OWNER TO postgres;

--
-- Name: watermeter_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.watermeter_id_seq OWNED BY public.watermeter.id;


--
-- Name: watermeterflat; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.watermeterflat (
                                       id integer NOT NULL,
                                       flat_id integer NOT NULL,
                                       watermeter_id integer NOT NULL
);


ALTER TABLE public.watermeterflat OWNER TO postgres;

--
-- Name: watermeterflat_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.watermeterflat_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.watermeterflat_id_seq OWNER TO postgres;

--
-- Name: watermeterflat_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.watermeterflat_id_seq OWNED BY public.watermeterflat.id;


--
-- Name: building id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.building ALTER COLUMN id SET DEFAULT nextval('public.building_id_seq'::regclass);


--
-- Name: company id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.company ALTER COLUMN id SET DEFAULT nextval('public.company_id_seq'::regclass);


--
-- Name: feetype id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.feetype ALTER COLUMN id SET DEFAULT nextval('public.feetype_id_seq'::regclass);


--
-- Name: flat id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.flat ALTER COLUMN id SET DEFAULT nextval('public.flat_id_seq'::regclass);


--
-- Name: heatmonthlycoefficient id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.heatmonthlycoefficient ALTER COLUMN id SET DEFAULT nextval('public.heatmonthlycoefficient_id_seq'::regclass);


--
-- Name: invoice id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.invoice ALTER COLUMN id SET DEFAULT nextval('public.invoice_id_seq'::regclass);


--
-- Name: occtenant id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.occtenant ALTER COLUMN id SET DEFAULT nextval('public.occtenant_id_seq'::regclass);


--
-- Name: occupancy id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.occupancy ALTER COLUMN id SET DEFAULT nextval('public.occupancy_id_seq'::regclass);


--
-- Name: payment id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.payment ALTER COLUMN id SET DEFAULT nextval('public.payment_id_seq'::regclass);


--
-- Name: paymenttype id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.paymenttype ALTER COLUMN id SET DEFAULT nextval('public.paymenttype_id_seq'::regclass);


--
-- Name: roomtype id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.roomtype ALTER COLUMN id SET DEFAULT nextval('public.roomtype_id_seq'::regclass);


--
-- Name: roomtypeflat id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.roomtypeflat ALTER COLUMN id SET DEFAULT nextval('public.roomtypeflat_id_seq'::regclass);


--
-- Name: tenant id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.tenant ALTER COLUMN id SET DEFAULT nextval('public.tenant_id_seq'::regclass);


--
-- Name: watermeasurement id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.watermeasurement ALTER COLUMN id SET DEFAULT nextval('public.watermeasurement_id_seq'::regclass);


--
-- Name: watermeter id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.watermeter ALTER COLUMN id SET DEFAULT nextval('public.watermeter_id_seq'::regclass);


--
-- Name: watermeterflat id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.watermeterflat ALTER COLUMN id SET DEFAULT nextval('public.watermeterflat_id_seq'::regclass);


--
-- Data for Name: building; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.building (id, street, no, npa, city) FROM stdin;
1	Rue des Greniers	1-3	1522	Lucens
\.


--
-- Data for Name: company; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.company (id, name, email, tel) FROM stdin;
2	JM Michoud	jm@michoud.ch	0244302378
3	groupe e	info@entretec.ch	0264667080
4	Pillnel Thermique	yp@pillonelthermique.ch	0266635680
5	Lucens	bourse@lucens.ch	0219061559
6	energiapro	client@energiapro.ch	0800429429
\.


--
-- Data for Name: feetype; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.feetype (id, type) FROM stdin;
1	Eau chaude & Chauffage
2	Epuration
3	Divers
\.


--
-- Data for Name: flat; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.flat (id, no, building_id) FROM stdin;
1	101	1
2	01	1
3	11	1
\.


--
-- Data for Name: heatmonthlycoefficient; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.heatmonthlycoefficient (id, month, coefficient) FROM stdin;
1	1	18
2	2	16
3	3	15
4	4	10
5	5	5
6	6	0
7	7	0
8	8	0
9	9	2
12	12	18
10	10	6
11	11	10
\.


--
-- Data for Name: invoice; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.invoice (id, amount, date, building_id, fee_type_id, company_id) FROM stdin;
1	3069	2023-08-31	1	1	6
2	3000	2023-10-31	1	1	6
3	2000	2023-12-31	1	1	6
4	3000	2024-02-29	1	1	6
5	2000	2024-04-01	1	1	6
6	171	2023-09-15	1	1	2
7	479	2024-01-31	1	1	3
8	398	2023-10-01	1	1	4
9	1685	2023-12-07	1	2	5
10	1501	2024-06-01	1	2	5
\.


--
-- Data for Name: occtenant; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.occtenant (id, occupancy_id, tenant_id) FROM stdin;
1	1	3
2	2	2
3	3	1
\.


--
-- Data for Name: occupancy; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.occupancy (id, start, rent, fees, ddm, flat_id, end_date) FROM stdin;
3	2024-01-01	850	150	7	2	\N
2	2023-07-01	1450	300	7	3	\N
1	2023-01-01	2200	350	7	1	2024-01-01
\.


--
-- Data for Name: payment; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.payment (id, date, amount, paymenttype_id, occupancy_id) FROM stdin;
\.


--
-- Data for Name: paymenttype; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.paymenttype (id, type) FROM stdin;
\.


--
-- Data for Name: roomtype; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.roomtype (id, coefficient, type) FROM stdin;
1	1	Salon
2	1	Chambre
\.


--
-- Data for Name: roomtypeflat; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.roomtypeflat (id, volume, room_type_id, flat_id) FROM stdin;
1	200	1	1
2	176	2	1
3	100	1	2
4	11	2	2
5	150	1	3
6	61	2	3
\.


--
-- Data for Name: tenant; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.tenant (id, firstname, lastname, email, tel) FROM stdin;
1	John	Doe	john@doe.com	89723984739
2	Marvin	Mark	marvin@mark.com	28726873263
3	Josephine	Pierre	josephine@pierre.com	78632786327
\.


--
-- Data for Name: watermeasurement; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.watermeasurement (id, year, measure, watermeter_id) FROM stdin;
\.


--
-- Data for Name: watermeter; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.watermeter (id, no) FROM stdin;
\.


--
-- Data for Name: watermeterflat; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.watermeterflat (id, flat_id, watermeter_id) FROM stdin;
\.


--
-- Name: building_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.building_id_seq', 1, true);


--
-- Name: company_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.company_id_seq', 6, true);


--
-- Name: feetype_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.feetype_id_seq', 3, true);


--
-- Name: flat_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.flat_id_seq', 3, true);


--
-- Name: heatmonthlycoefficient_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.heatmonthlycoefficient_id_seq', 12, true);


--
-- Name: invoice_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.invoice_id_seq', 10, true);


--
-- Name: occtenant_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.occtenant_id_seq', 3, true);


--
-- Name: occupancy_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.occupancy_id_seq', 3, true);


--
-- Name: payment_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.payment_id_seq', 1, false);


--
-- Name: paymenttype_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.paymenttype_id_seq', 1, false);


--
-- Name: roomtype_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.roomtype_id_seq', 2, true);


--
-- Name: roomtypeflat_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.roomtypeflat_id_seq', 6, true);


--
-- Name: tenant_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.tenant_id_seq', 3, true);


--
-- Name: watermeasurement_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.watermeasurement_id_seq', 1, false);


--
-- Name: watermeter_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.watermeter_id_seq', 1, false);


--
-- Name: watermeterflat_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.watermeterflat_id_seq', 1, false);


--
-- Name: building building_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.building
    ADD CONSTRAINT building_pkey PRIMARY KEY (id);


--
-- Name: company company_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.company
    ADD CONSTRAINT company_pkey PRIMARY KEY (id);


--
-- Name: feetype feetype_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.feetype
    ADD CONSTRAINT feetype_pkey PRIMARY KEY (id);


--
-- Name: flat flat_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.flat
    ADD CONSTRAINT flat_pkey PRIMARY KEY (id);


--
-- Name: heatmonthlycoefficient heatmonthlycoefficient_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.heatmonthlycoefficient
    ADD CONSTRAINT heatmonthlycoefficient_pkey PRIMARY KEY (id);


--
-- Name: invoice invoice_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.invoice
    ADD CONSTRAINT invoice_pkey PRIMARY KEY (id);


--
-- Name: occtenant occtenant_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.occtenant
    ADD CONSTRAINT occtenant_pkey PRIMARY KEY (id);


--
-- Name: occupancy occupancy_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.occupancy
    ADD CONSTRAINT occupancy_pkey PRIMARY KEY (id);


--
-- Name: payment payment_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.payment
    ADD CONSTRAINT payment_pkey PRIMARY KEY (id);


--
-- Name: paymenttype paymenttype_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.paymenttype
    ADD CONSTRAINT paymenttype_pkey PRIMARY KEY (id);


--
-- Name: roomtype roomtype_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.roomtype
    ADD CONSTRAINT roomtype_pkey PRIMARY KEY (id);


--
-- Name: roomtypeflat roomtypeflat_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.roomtypeflat
    ADD CONSTRAINT roomtypeflat_pkey PRIMARY KEY (id);


--
-- Name: tenant tenant_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.tenant
    ADD CONSTRAINT tenant_pkey PRIMARY KEY (id);


--
-- Name: watermeasurement watermeasurement_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.watermeasurement
    ADD CONSTRAINT watermeasurement_pkey PRIMARY KEY (id);


--
-- Name: watermeter watermeter_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.watermeter
    ADD CONSTRAINT watermeter_pkey PRIMARY KEY (id);


--
-- Name: watermeterflat watermeterflat_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.watermeterflat
    ADD CONSTRAINT watermeterflat_pkey PRIMARY KEY (id);


--
-- Name: flat flat_building_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.flat
    ADD CONSTRAINT flat_building_id_fkey FOREIGN KEY (building_id) REFERENCES public.building(id) ON DELETE CASCADE;


--
-- Name: invoice invoice_building_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.invoice
    ADD CONSTRAINT invoice_building_id_fkey FOREIGN KEY (building_id) REFERENCES public.building(id) ON DELETE CASCADE;


--
-- Name: invoice invoice_company_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.invoice
    ADD CONSTRAINT invoice_company_id_fkey FOREIGN KEY (company_id) REFERENCES public.company(id) ON DELETE CASCADE;


--
-- Name: invoice invoice_fee_type_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.invoice
    ADD CONSTRAINT invoice_fee_type_id_fkey FOREIGN KEY (fee_type_id) REFERENCES public.feetype(id) ON DELETE CASCADE;


--
-- Name: occtenant occtenant_occupancy_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.occtenant
    ADD CONSTRAINT occtenant_occupancy_id_fkey FOREIGN KEY (occupancy_id) REFERENCES public.occupancy(id) ON DELETE CASCADE;


--
-- Name: occtenant occtenant_tenant_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.occtenant
    ADD CONSTRAINT occtenant_tenant_id_fkey FOREIGN KEY (tenant_id) REFERENCES public.tenant(id) ON DELETE CASCADE;


--
-- Name: occupancy occupancy_flat_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.occupancy
    ADD CONSTRAINT occupancy_flat_id_fkey FOREIGN KEY (flat_id) REFERENCES public.flat(id) ON DELETE CASCADE;


--
-- Name: payment payment_occupancy_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.payment
    ADD CONSTRAINT payment_occupancy_id_fkey FOREIGN KEY (occupancy_id) REFERENCES public.occupancy(id) ON DELETE CASCADE;


--
-- Name: payment payment_paymenttype_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.payment
    ADD CONSTRAINT payment_paymenttype_id_fkey FOREIGN KEY (paymenttype_id) REFERENCES public.paymenttype(id) ON DELETE CASCADE;


--
-- Name: roomtypeflat roomtypeflat_flat_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.roomtypeflat
    ADD CONSTRAINT roomtypeflat_flat_id_fkey FOREIGN KEY (flat_id) REFERENCES public.flat(id) ON DELETE CASCADE;


--
-- Name: roomtypeflat roomtypeflat_room_type_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.roomtypeflat
    ADD CONSTRAINT roomtypeflat_room_type_id_fkey FOREIGN KEY (room_type_id) REFERENCES public.roomtype(id) ON DELETE CASCADE;


--
-- Name: watermeasurement watermeasurement_watermeter_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.watermeasurement
    ADD CONSTRAINT watermeasurement_watermeter_id_fkey FOREIGN KEY (watermeter_id) REFERENCES public.watermeter(id) ON DELETE CASCADE;


--
-- Name: watermeterflat watermeterflat_flat_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.watermeterflat
    ADD CONSTRAINT watermeterflat_flat_id_fkey FOREIGN KEY (flat_id) REFERENCES public.flat(id) ON DELETE CASCADE;


--
-- Name: watermeterflat watermeterflat_watermeter_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.watermeterflat
    ADD CONSTRAINT watermeterflat_watermeter_id_fkey FOREIGN KEY (watermeter_id) REFERENCES public.watermeter(id) ON DELETE CASCADE;


--
-- PostgreSQL database dump complete
--
