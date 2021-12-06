# How to change Storage kind <br> <스토리지 저장 파일 종류 바꾸기>
> 각 스토리지의 `Storage_<>/storage.c` 파일의 42번쨰 라인
> ```c
>  strcpy(storageMyInfo.stor_kind, "<원하는 스토리지 종류>"); //수정
>  strcpy(storageMyInfo.stor_kind, "Image"); //예시
> ```
> 와 같이 수정

[돌아가기](./README.md)