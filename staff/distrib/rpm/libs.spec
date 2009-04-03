%define _topdir  __topdir__

Summary: staff
Name: staff
Version: __version__
Release: mcbc
Copyright: 2009
Group: Utilities/System
Requires: rise postgresql-libs
Provides: staff

%description
staff

%files
%defattr(-,root,root)
/etc/profile.d/axis2c.sh
/etc/profile.d/staff.sh
/etc/ld.so.conf.d/axis2c.conf
/etc/ld.so.conf.d/staff.conf
/usr/local/staff/lib/
/usr/local/staff/components/
/usr/local/staff/staff.xml
/usr/local/staff/db/
/usr/local/staff/samples/
/usr/local/axis2c/

%pre

%preun
echo -n "�������� ��..."
cd /usr/local/staff/ && psql -q -U postgres template1 < db/uninstdb.sql && echo "     ��"

%post
ldconfig
echo -n "��������� ��..."
cd /usr/local/staff/ && psql -q -U postgres template1 < db/instdb.sql >/dev/null && echo "     ��"
cat << CONFIG_MESSAGE_END
��� ���������� ��������� staff:
 1) �������� ��������� ������ � /usr/local/axis2c/axis2.xml:
 
    <module ref="staff"/>
    <module ref="staff_security"/>
    
    ������ �� �������:
    <module ref="addressing"/>

 2) �������������� ���� ��������� ������� � ��:
 
    mcedit /var/lib/pgsql/data/pg_hba.conf

    �������������� ������, ������� � ţ ������ ������ '#'

    #local  all      all        127.0.0.1    255.255.255.255   pam
    
    �������� ������
    
    local  all      all        127.0.0.1    255.255.255.255   md5

CONFIG_MESSAGE_END

%postun
