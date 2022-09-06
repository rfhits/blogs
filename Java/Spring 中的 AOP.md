转自[AOP-睿站-狂神说](https://www.bilibili.com/video/BV1WE411d7Dv?p=20)

AOP 指的是面向切面编程

常用的场景是通过动态代理的对象，在原有的方法前后加上新的方法。

比如要在某个 service 的前后，增加日志功能。

可以写个类，实现 `MethodBeforeAdvice` 或 `MethodAfterAdvice`

然后在 xml 中定义切入点等配置，那么在执行对应的方法时，就会有方法“环绕”在其周围。

我们还可以通过注解实现 AOP ，这样比较易懂。

```java
import org.aspectj.lang.annotation.After;
import org.aspectj.lang.annotation.Around;
import org.aspectj.lang.annotation.Aspect;
import org.aspectj.lang.annotation.Before;
@Aspect //标注这个类是一个切面
public class AnnotationPointcut {

    @Before("execution(* com.kuang.service.UserServiceImpl.*(..))")
    public void before(){
        System.out.println("=====方法执行前======");
    }
    
    @After("execution(* com.kuang.service.UserServiceImpl.*(..))")
    public void after(){
        System.out.println("=====方法执行后======");
    }

    //在环绕增强中，我们可以给定一个参数，代表我们要获取处理切入的点;
    @Around("execution(* com.kuang.service.UserServiceImpl.*(..))")
    public void around(ProceedingJoinPoint jp) throws Throwable {
        System.out.println("环绕前");
        object proceed = jp.proceed();
        //执行方法System.out.print1n("环绕后");
        //signature signature = jp.getsignature();//获得签名
        //System.out.printLn( "signature : "+signature);
        //System.out.println(proceed);
    }
}
```