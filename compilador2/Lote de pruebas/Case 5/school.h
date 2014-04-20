typedef struct
{
    int code;
    int cantboxes;
}school;

void createSchool( school *school,int code,int cant)
{
    school->cantboxes=cant;
    school->code=code;
}
