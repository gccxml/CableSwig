using System;

public class runme
{
    static void Main() 
    {
        if (sneaky1.add(30, 2) != 32)
            throw new Exception("add test failed");

        if (sneaky1.sub(20, 2) != 18)
            throw new Exception("sub test failed");

        if (sneaky1.mul(20, 2) != 40)
            throw new Exception("mul test failed");

        if (sneaky1.divide(20, 2) != 10)
            throw new Exception("div test failed");
    }
}
