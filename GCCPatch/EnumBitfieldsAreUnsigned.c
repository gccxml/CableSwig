enum et { e128 = 128 };

int main()
{            
  struct { enum et value:8; } d;
  d.value = e128;
  if(d.value < 0)
    {
    return 0;
    }
  else
    {
    return 1;
    }
}
