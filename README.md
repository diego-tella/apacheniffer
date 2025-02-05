# apacheniffer
Backdoor module to apache2 that sniff requests and execute commands.

Still in developmeant. 

<h2>Executing commands</h2>

![image](https://github.com/user-attachments/assets/e83d872b-bc2c-4e87-8e40-be128e9349f8)

<h2>Saving requests</h2>

![image](https://github.com/user-attachments/assets/96139402-f9ca-4eaf-880e-064143c17afb)


# Install

```
apxs -c -i -a apacheniffer_module.c
systemctl restart apache2
```
