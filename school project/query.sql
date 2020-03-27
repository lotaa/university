select * from student;
select s.name , d.fname 
from student s , dependent d
where s.ssn=d.student_ssn;
select s.name 
from student s ,class c 
where c.no=s.class_no and c.no='3/2';
select e.ename , c.no
from teacher t ,teach2 te ,  class c ,employee e 
where t.employee_ssn=te.teacher_employee_ssn and c.no=te.class_no and e.ssn=t.employee_ssn;
select w.specialization , e.ename
from employee e, worker w 
where e.ssn=w.employee_ssn;
select e.ssn, e.ename , sb.name 
from teacher t , employee e , subject sb 
where sb.no=t.subject_no and e.ssn=t.employee_ssn and sb.name='arabic' ;
select s.name , s.year ,g.type
from student s , grade g 
where s.ssn=g.student_ssn and g.type='pass';
select count(e.ename) 
from employee e,manger m
where e.ssn=m.employee_ssn;
select s.name as student , e.ename as teacher 
from employee e , teacher t , student s , teach2 te,class c 
where e.ssn=t.employee_ssn and t.employee_ssn=te.teacher_employee_ssn and c.no=te.class_no and s.class_no=c.no;
select count(s.ssn), c.no
from student s , grade g , class c 
where s.ssn=g.student_ssn and c.no=s.class_no and g.type='pass'
group by c.no;
select sum(st.degree) , s.name
from student s , study st , subject sb
where sb.no=st.subject_no and s.ssn=st.student_ssn 
group by s.name; 